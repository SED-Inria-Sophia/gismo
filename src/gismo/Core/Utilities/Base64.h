#pragma once

#include <gismo/Common/DebugAssert.h>
#include <gismo/Math/LinearAlgebra.h>

#include <array>
#include <string>
#include <vector>

namespace gismo {

/**
 * @brief Encode for base64 export
 *
 * Static class to provide functionality to encode any type of vector into an
 * ascii string (uncompressed) and its inverse operation.
 *
 */
class GISMO_EXPORT Base64 {
   private:
    /// Alias for one byte type
    using ByteRepresentation = unsigned char;

    /// Look up table
    static char char_encode_table(const unsigned& index);

    /**
     * @brief Reverse the encoding table in an array
     *
     * Chars are casted to unsigned types for indexing, function will only be
     * called once
     *
     * @return std::array<unsigned, 256>
     */
    static const std::array<unsigned, 256> ReverseCharEncodeTable_();

    /**
     * @brief Reverse table lookup for char decoding
     */
    static unsigned char_decode_table(const unsigned& index);

    /**
     * @brief Check if a base64 string is valid
     */
    static bool isValidBase64String(const std::string& s);

    /**
     * @brief Trim whitespaces from string
     */
    static std::string trimWhitespaces(const std::string& s);

    /**
     * @brief Internal encoding function
     *
     * @param byte_vector_ptr pointer to start of an array (contiguous)
     * @param minimum_n_bytes_required number of bytes to encode
     * @return std::string encoded data
     */
    static std::string Encode_(const ByteRepresentation* byte_vector_ptr,
                               const std::size_t& minimum_n_bytes_required);

    /**
     * @brief Copy a read vector of type BaseType into a gsMatrix with
     * ScalarType TargetType.
     *
     * This is required as they might differ from each other, e.g., double into
     * float, uint into int, etc.. Further, most matrices in Gismo (like
     * coefficients) are stored in a different order (colwise) than the input
     * stream, which prohibits the use of writing directly on the pointer
     *
     * @tparam BaseType Type as encoded into the input file
     * @tparam TargetType Type as requested from target
     * @param base_vector decoded std::vector with base type
     * @param result gsMatrix passed as reference
     */
    template <typename BaseType, typename TargetType>
    static void CopyIntoGsMatrix(const std::vector<BaseType>& base_vector,
                                 gsMatrix<TargetType>& result);

    /**
     * @brief Cast a vector of a base type into a vector of TargetType
     *
     * @tparam BaseType Type as encoded into the input file
     * @tparam TargetType Type as requested from target
     * @param base_vector decoded std::vector with base type
     * @param result decoded std::vector with target type
     */
    template <typename BaseType, typename TargetType>
    static void CopyIntoVector(const std::vector<BaseType>& base_vector,
                               std::vector<TargetType>& result);

   public:
    /**
     * @brief Helper routine for std::vector data
     *
     * @tparam BaseType type of individual data entries
     * @param data_vector data to be encoded
     * @return std::string encoded data
     */
    template <typename BaseType>
    static std::string Encode(const std::vector<BaseType>& data_vector);

    /**
     * @brief Helper routine for gsMatrix Types (non-sparse)
     *
     * @tparam BaseType type of individual data entries
     * @param data_vector data to be encoded
     * @param row_wise Encode in row_wise style
     * @return std::string encoded data
     */
    template <typename BaseType>
    static std::string Encode(const gsMatrix<BaseType>& data_vector,
                              const bool& row_wise = true);

    /**
     * @brief Reading a B64 string, transforming it into a vector of a
     * specific type
     *
     * @todo: In the future copies could be avoided by using string_view
     *
     * @tparam OutputType target type
     */
    template <typename OutputType>
    static std::vector<OutputType> Decode(const std::string& base64string);

    /**
     * @brief Decode a string and copy into requested gismo Type
     *
     * Read a base64 string depending on a format flag (that determines base
     * type) and write it into gsMatrix<ScalarType>. Other overloads provide
     * functions to copy into a std::vector<ScalarType>
     *
     * @tparam ScalarType
     * @param base64_string
     * @param base_type_flag_
     * @param result
     */
    template <typename ScalarType>
    static void DecodeIntoGsType(const std::string& base64_string,
                                 const std::string& base_type_flag_,
                                 gsMatrix<ScalarType>& result);

    /**
     * @brief Decode a string and copy into requested gismo Type
     *
     * Read a base64 string depending on a format flag (that determines base
     * type) and write it into std::vector<ScalarType>. Other overloads provide
     * functions to copy into a gsMatrix<ScalarType>
     *
     * @tparam ScalarType
     * @param base64_string
     * @param base_type_flag_
     * @param result
     */
    template <typename ScalarType>
    static void DecodeIntoGsType(const std::string& base64_string,
                                 const std::string& base_type_flag_,
                                 std::vector<ScalarType>& result);
};

}  // namespace gismo

// Include template implementations
#include <gismo/Core/Utilities/Base64.hpp>