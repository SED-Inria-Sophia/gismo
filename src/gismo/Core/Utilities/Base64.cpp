#include <gismo/Core/Utilities/Base64.h>

#include <algorithm>
#include <array>
#include <numeric>

namespace gismo {

char Base64::char_encode_table(const unsigned& index) {
    // Create static array in function to avoid use of c++17 static member
    // declaration
    static const std::array<char, 64> encode_table{
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};
    GISMO_ASSERT((index < 64),
                 "Requested index out of range. Input invalid");
    return encode_table[index];
}

const std::array<unsigned, 256> Base64::ReverseCharEncodeTable_() {
    std::array<unsigned, 256> et_reversed{};
    // Fill remainding array entries with values outside range to throw
    // assertions easily
    et_reversed.fill(256);
    const unsigned n_chars_{64};
    for (unsigned i{}; i < n_chars_; i++) {
        et_reversed[static_cast<unsigned>(char_encode_table(i))] = i;
    }
    return et_reversed;
}

unsigned Base64::char_decode_table(const unsigned& index) {
    static const std::array<unsigned, 256> & decode_table =
        ReverseCharEncodeTable_();
    GISMO_ASSERT((index < 256),
                 "Requested index out of range. Input invalid");
    GISMO_ASSERT((decode_table[index] != 256),
                 "Invalid decode type, this should never occur!");
    return decode_table[index];
}

bool Base64::isValidBase64String(const std::string& s) {
    // Check if size is feasible
    bool is_valid{s.size() % 4 == 0};
    // Check if string only contains alpha-numeric chars or `/`, `+` or `=`
    return is_valid && std::all_of(s.begin(), s.end(), [](const char& c) {
               return isalnum(c) || c == '+' || c == '/' || c == '=';
           });
}

std::string Base64::trimWhitespaces(const std::string& s) {
    const std::string delimiters(" \n\t");
    size_t first = s.find_first_not_of(delimiters);
    if (std::string::npos == first) {
        GISMO_ERROR("Empty string cannot be converted into data-vector");
    }
    size_t last = s.find_last_not_of(delimiters);
    return s.substr(first, (last - first + 1));
}

std::string Base64::Encode_(const ByteRepresentation* byte_vector_ptr,
                           const std::size_t& minimum_n_bytes_required) {
    // Padding blocks
    const std::size_t additional_padding_bytes =
        (3 - minimum_n_bytes_required % 3) % 3;

    // Required groups of three
    const std::size_t number_of_groups =
        (minimum_n_bytes_required + additional_padding_bytes) / 3;

    // Initialize return value
    std::string encoded_string;
    encoded_string.reserve(number_of_groups * 4);

    // Loop over bytes and decode them
    for (std::size_t i_group{}; i_group < number_of_groups; i_group++) {
        const std::size_t buffer_index = i_group * 3;
        std::array<ByteRepresentation, 3> buffer{};
        buffer[0] = buffer_index < minimum_n_bytes_required
                        ? byte_vector_ptr[buffer_index + 0]
                        : 0;
        buffer[1] = buffer_index < minimum_n_bytes_required
                        ? byte_vector_ptr[buffer_index + 1]
                        : 0;
        buffer[2] = buffer_index < minimum_n_bytes_required
                        ? byte_vector_ptr[buffer_index + 2]
                        : 0;

        // Transform bytes into chars using above private encoder table
        encoded_string.push_back(
            char_encode_table(((buffer[0] & 0xfc) >> 2)));
        encoded_string.push_back(char_encode_table(
            ((buffer[0] & 0x03) << 4) + ((buffer[1] & 0xf0) >> 4)));
        encoded_string.push_back(char_encode_table(
            ((buffer[1] & 0x0f) << 2) + ((buffer[2] & 0xc0) >> 6)));
        encoded_string.push_back(
            char_encode_table(((buffer[2] & 0x3f) << 0)));
    }

    // Replace trailing invalid data with `=`
    for (size_t i = 0; i < additional_padding_bytes; ++i) {
        encoded_string[number_of_groups * 4 - i - 1] = '=';
    }

    // Safety check
    GISMO_ASSERT(
        isValidBase64String(encoded_string),
        "Something went wrong in B64 encoding, please write an issue");
    return encoded_string;
}

}  // namespace gismo