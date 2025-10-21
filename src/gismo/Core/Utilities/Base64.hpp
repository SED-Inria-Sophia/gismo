#pragma once

#include <algorithm>
#include <numeric>

namespace gismo {

// Template implementations for Base64 class

template <typename BaseType, typename TargetType>
void Base64::CopyIntoGsMatrix(const std::vector<BaseType>& base_vector,
                             gsMatrix<TargetType>& result) {
    // Check for size
    const unsigned rows = result.rows();
    const unsigned cols = result.cols();
    // Size check
    if (base_vector.size() != (rows * cols)) {
        GISMO_ERROR(
            "Input array has the wrong size or could not be converted");
    }
    // Converting into gsMatrix (manipulating directly on gsMatrix is
    // more efficient if T=InputType)
    for (unsigned i = 0; i < rows; ++i) {
        for (unsigned j = 0; j < cols; ++j) {
            result(i, j) =
                static_cast<TargetType>(base_vector[i * cols + j]);
        }
    }
}

template <typename BaseType, typename TargetType>
void Base64::CopyIntoVector(const std::vector<BaseType>& base_vector,
                           std::vector<TargetType>& result) {
    std::transform(base_vector.cbegin(), base_vector.cend(),
                   std::back_inserter(result), [](const BaseType& c) {
                       return static_cast<TargetType>(c);
                   });
}

template <typename BaseType>
std::string Base64::Encode(const std::vector<BaseType>& data_vector) {
    const ByteRepresentation* vector_as_bytes_ptr =
        reinterpret_cast<const ByteRepresentation*>(&data_vector[0]);

    // Number of bytes for an entry
    constexpr const std::size_t length_of_entry{sizeof(BaseType{})};
    // Minimum number of bytes required
    const std::size_t minimum_n_bytes_required =
        length_of_entry * data_vector.size();

    return Encode_(vector_as_bytes_ptr, minimum_n_bytes_required);
}

template <typename BaseType>
std::string Base64::Encode(const gsMatrix<BaseType>& data_vector,
                          const bool& row_wise) {
    GISMO_ASSERT(std::is_arithmetic<BaseType>::value, // can be static
                  "Encoding is unsafe for non-arithmetic types.");
    // We need to ensure that the export is in the demanded export order, if
    // the data is only a vector (i.e. either col or row is 1) or if the
    // storage scheme is coherent with the demanded export order
    if ((data_vector.cols() == 1) || (data_vector.rows() == 1) ||
        (gsMatrix<BaseType>::IsRowMajor == row_wise)) {
        const ByteRepresentation* vector_as_bytes_ptr =
            reinterpret_cast<const ByteRepresentation*>(data_vector.data());

        // Number of bytes for an entry
        constexpr const std::size_t length_of_entry{sizeof(BaseType{})};
        // Minimum number of bytes required
        const std::size_t minimum_n_bytes_required =
            length_of_entry * data_vector.size();

        return Encode_(vector_as_bytes_ptr, minimum_n_bytes_required);
    } else {
        // Here we need to flip the order of elements, for simplicity the
        // data is copied into a temporary vector (works best with current
        // implementation but might be slower than other approaches)
        std::vector<BaseType> copy_of_matrix;
        copy_of_matrix.reserve(data_vector.rows() * data_vector.cols());

        // For readability we manipulate the Matrix using (memory safe)
        // operator() overloads
        if (row_wise) {
            for (index_t i = 0; i < data_vector.rows(); ++i) {
                for (index_t j = 0; j < data_vector.cols(); ++j) {
                    copy_of_matrix.push_back(data_vector(i, j));
                }
            }
        } else {
            for (index_t j = 0; j < data_vector.cols(); ++j) {
                for (index_t i = 0; i < data_vector.rows(); ++i) {
                    copy_of_matrix.push_back(data_vector(i, j));
                }
            }
        }
        // Use vector overload
        return Encode(copy_of_matrix);
    }
}

template <typename OutputType>
std::vector<OutputType> Base64::Decode(const std::string& base64string) {
    // Safeguard
    const std::string& base64string_trimmed = trimWhitespaces(base64string);
    // Check validity of string
    GISMO_ASSERT(isValidBase64String(base64string_trimmed),
                 "Validity check failed");

    // Init return value
    const std::size_t number_of_groups{base64string_trimmed.size() / 4};
    constexpr const std::size_t length_of_entry{sizeof(OutputType{})};
    const std::size_t number_of_output_values{(number_of_groups * 3) /
                                              length_of_entry};
    std::vector<OutputType> return_value;
    return_value.resize(number_of_output_values);

    // Access as byte stream
    ByteRepresentation* vector_as_bytes =
        reinterpret_cast<ByteRepresentation*>(&return_value[0]);

    // Start the reverse process
    for (std::size_t i_group{}; i_group < number_of_groups; i_group++) {
        const std::size_t buffer_index = i_group * 4;
        std::array<unsigned, 4> buffer{};
        for (unsigned i{}; i < 4; i++) {
            buffer[i] = base64string_trimmed[buffer_index + i] != '='
                            ? char_decode_table(static_cast<unsigned>(
                                  base64string_trimmed[buffer_index + i]))
                            : 255;
        }

        // Write bytes into vector
        if (buffer[1] != 255) {
            vector_as_bytes[i_group * 3] =
                ((buffer[0] & 0x3f) << 2) + ((buffer[1] & 0x30) >> 4);
        }
        if (buffer[2] != 255) {
            vector_as_bytes[i_group * 3 + 1] =
                ((buffer[1] & 0x0f) << 4) + ((buffer[2] & 0x3c) >> 2);
        }
        if (buffer[3] != 255) {
            vector_as_bytes[i_group * 3 + 2] =
                ((buffer[2] & 0x03) << 6) + ((buffer[3] & 0x3f) >> 0);
        }
    }
    return return_value;
}

template <typename ScalarType>
void Base64::DecodeIntoGsType(const std::string& base64_string,
                             const std::string& base_type_flag_,
                             gsMatrix<ScalarType>& result) {
    // Format flag in this function is case sensitive
    GISMO_ASSERT(
        std::none_of(base_type_flag_.begin(), base_type_flag_.end(),
                     isupper),
        "Format flag {ascii, b64float64, ...} must be all lowercase.");

    // Perform type checks (no integral to floting point conversion)
    if (std::is_integral<ScalarType>::value ^
        (base_type_flag_.find("int") != std::string::npos)) {
        GISMO_ERROR(
            "Conversions from integral to floating type and vice-versa is "
            "not allowed!");
    }

    // Perform the actual input (using the proper encoding type)
    if (base_type_flag_ == "b64uint16") {  // Unsigned int 16
        CopyIntoGsMatrix(Decode<uint16_t>(base64_string), result);
    } else if (base_type_flag_ == "b64uint32") {  // Unsigned int 32
        CopyIntoGsMatrix(Decode<uint32_t>(base64_string), result);
    } else if (base_type_flag_ == "b64bint64") {  // Unsigned int 64
        CopyIntoGsMatrix(Decode<uint64_t>(base64_string), result);
    } else if (base_type_flag_ == "b64int16") {  // Int 16
        CopyIntoGsMatrix(Base64::Decode<int16_t>(base64_string), result);
    } else if (base_type_flag_ == "b64int32") {  // Int 32
        CopyIntoGsMatrix(Base64::Decode<int32_t>(base64_string), result);
    } else if (base_type_flag_ == "b64int64") {  // Int 64
        CopyIntoGsMatrix(Base64::Decode<int64_t>(base64_string), result);
    } else if (base_type_flag_ == "b64float32") {  // Float 32
        CopyIntoGsMatrix(Base64::Decode<float>(base64_string), result);
    } else if (base_type_flag_ == "b64float64") {  // Float 64
        CopyIntoGsMatrix(Base64::Decode<double>(base64_string), result);
    } else {
        GISMO_ERROR("Reading matrix from XML found unknown type");
    }
}

template <typename ScalarType>
void Base64::DecodeIntoGsType(const std::string& base64_string,
                             const std::string& base_type_flag_,
                             std::vector<ScalarType>& result) {
    // Format flag in this function is case sensitive
    GISMO_ASSERT(
        std::none_of(base_type_flag_.begin(), base_type_flag_.end(),
                     isupper),
        "Format flag {ascii, b64float64, ...} must be all lowercase.");

    // Perform type checks (no integral to floting point conversion)
    if (std::is_integral<ScalarType>::value ^
        (base_type_flag_.find("int") != std::string::npos)) {
        GISMO_ERROR(
            "Conversions from integral to floating type and vice-versa is "
            "not allowed!");
    }

    // Perform the actual input (using the proper encoding type)
    if (base_type_flag_ == "b64uint16") {  // Unsigned int 16
        CopyIntoVector(Decode<uint16_t>(base64_string), result);
    } else if (base_type_flag_ == "b64uint32") {  // Unsigned int 32
        CopyIntoVector(Decode<uint32_t>(base64_string), result);
    } else if (base_type_flag_ == "b64bint64") {  // Unsigned int 64
        CopyIntoVector(Decode<uint64_t>(base64_string), result);
    } else if (base_type_flag_ == "b64int16") {  // Int 16
        CopyIntoVector(Base64::Decode<int16_t>(base64_string), result);
    } else if (base_type_flag_ == "b64int32") {  // Int 32
        CopyIntoVector(Base64::Decode<int32_t>(base64_string), result);
    } else if (base_type_flag_ == "b64int64") {  // Int 64
        CopyIntoVector(Base64::Decode<int64_t>(base64_string), result);
    } else if (base_type_flag_ == "b64float32") {  // Float 32
        CopyIntoVector(Base64::Decode<float>(base64_string), result);
    } else if (base_type_flag_ == "b64float64") {  // Float 64
        CopyIntoVector(Base64::Decode<double>(base64_string), result);
    } else {
        GISMO_ERROR("Reading matrix from XML found unknown type");
    }
}

}  // namespace gismo