/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 15:14:46 by mkhan             #+#    #+#             */
/*   Updated: 2023/08/31 18:03:24 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <sstream>

// int main() {
//     // Sample HTTP response with chunked encoding
//     std::string http_response = "HTTP/1.1 200 OK\r\n"
//                                 "Content-Type: text/plain\r\n"
//                                 "Transfer-Encoding: chunked\r\n"
//                                 "\r\n"
//                                 "25\r\n"
//                                 "This is the data in chunk 1.\r\n"
//                                 "1A\r\n"
//                                 "And this is the data in chunk 2.\r\n"
//                                 "10\r\n"
//                                 "Final chunk!\r\n"
//                                 "0\r\n";

//     // Find the position of the empty line separating headers and body
//     size_t bodyStart = http_response.find("\r\n\r\n");
//     if (bodyStart == std::string::npos) {
//         std::cerr << "Failed to find body start" << std::endl;
//         return 1;
//     }

//     // Extract the body content
//     std::string bodyContent = http_response.substr(bodyStart + 4);  // +4 to skip \r\n\r\n

//     // Process chunked encoding
//     std::stringstream cleanedBody;
//     size_t startPos = 0;

//     while (startPos < bodyContent.length()) {
//         // Find the chunk size delimiter "\r\n"
//         size_t sizeDelimPos = bodyContent.find("\r\n", startPos);
//         if (sizeDelimPos == std::string::npos) {
//             break;  // No more chunks found
//         }

//         // Find the chunk size in hexadecimal
//         std::string chunkSizeHex = bodyContent.substr(startPos, sizeDelimPos - startPos);
//         size_t chunkSize = strtoul(chunkSizeHex.c_str(), NULL, 16);

//         // Move startPos to the beginning of the chunk data
//         startPos = sizeDelimPos + 2;  // Skip "\r\n"

//         // Append the chunk data to the cleaned body
//         cleanedBody << bodyContent.substr(startPos, chunkSize);

//         // Move startPos to the end of the current chunk data (skip the chunk data and "\r\n")
//         startPos += chunkSize + 2;  // Skip chunk data and "\r\n"
//     }

//     std::cout << cleanedBody.str() << std::endl;

//     return 0;
// }



#include <iostream>
#include <string>

// int main() {
//     // Replace this with your actual HTTP response string with chunked encoding
//     std::string httpResponse = "HTTP/1.1 200 OK\r\n"
//                                "Transfer-Encoding: chunked\r\n"
//                                "\r\n"
//                                "4\r\n"
//                                "This\r\n"
//                                "6\r\n"
//                                " is the\r\n"
//                                "7\r\n"
//                                " response\r\n"
//                                "0\r\n"
//                                "\r\n";

//     // Find the position of the empty line that separates headers from body
//     size_t bodyStart = httpResponse.find("\r\n\r\n");
//     if (bodyStart != std::string::npos) {
//         // Extract the chunked body content
//         std::string responseBody;
//         size_t chunkStart = bodyStart + 4;
//         while (chunkStart < httpResponse.size()) {
//             // Find the size of the current chunk
//             size_t chunkSizeEnd = httpResponse.find("\r\n", chunkStart);
//             if (chunkSizeEnd == std::string::npos) {
//                 std::cout << "Error: Invalid chunk format." << std::endl;
//                 return 1;
//             }
//             int chunkSize = std::stoi(httpResponse.substr(chunkStart, chunkSizeEnd - chunkStart), 0, 16);

//             // Extract and append the chunk data
//             size_t chunkDataStart = chunkSizeEnd + 2; // Skip "\r\n"
//             responseBody += httpResponse.substr(chunkDataStart, chunkSize);
            
//             // Move to the start of the next chunk
//             chunkStart = chunkDataStart + chunkSize + 2; // Skip chunk data and "\r\n"
//         }
        
//         std::cout << "Response Body:\n" << responseBody << std::endl;
//     } else {
//         std::cout << "Error: Could not find the body in the response." << std::endl;
//     }

//     return 0;
// }

// #include <iostream>
// #include <string>
// #include <sstream>
// #include <stdexcept>

// int main() {
//     // Replace this with your actual HTTP response string with chunked encoding
//     std::string httpResponse = "HTTP/1.1 200 OK\r\n"
//                                "Transfer-Encoding: chunked\r\n"
//                                "\r\n"
//                                "4\r\n"
//                                "This\r\n"
//                                "6\r\n"
//                                " is the\r\n"
//                                "7\r\n"
//                                " response\r\n"
//                                "0\r\n"
//                                "\r\n";

//     // Find the position of the empty line that separates headers from body
//     size_t bodyStart = httpResponse.find("\r\n\r\n");
//     if (bodyStart != std::string::npos) {
//         // Extract the chunked body content
//         std::string responseBody;
//         size_t chunkStart = bodyStart + 4;
//         while (chunkStart < httpResponse.size()) {
//             // Find the size of the current chunk
//             size_t chunkSizeEnd = httpResponse.find("\r\n", chunkStart);
//             if (chunkSizeEnd == std::string::npos) {
//                 std::cout << "Error: Invalid chunk format." << std::endl;
//                 return 1;
//             }
            
//             // Extract and convert the chunk size to integer
//             std::string chunkSizeHex = httpResponse.substr(chunkStart, chunkSizeEnd - chunkStart);
//             int chunkSize;
//             std::stringstream ss(chunkSizeHex);
//             ss >> std::hex >> chunkSize;

//             // Extract and append the chunk data
//             size_t chunkDataStart = chunkSizeEnd + 2; // Skip "\r\n"
//             responseBody += httpResponse.substr(chunkDataStart, chunkSize);
            
//             // Move to the start of the next chunk
//             chunkStart = chunkDataStart + chunkSize + 2; // Skip chunk data and "\r\n"
//         }
        
//         std::cout << "Response Body:\n" << responseBody << std::endl;
//     } else {
//         std::cout << "Error: Could not find the body in the response." << std::endl;
//     }

//     return 0;
// }


// #include <iostream>
// #include <string>
// #include <sstream>
// #include <vector>
// #include <cstdlib> // For strtol

// int main() {
//     std::string httpResponse = "HTTP/1.1 200 OK\r\n"
//                                "Transfer-Encoding: chunked\r\n"
//                                "\r\n"
//                                "4\r\n"
//                                "This\r\n"
//                                "6\r\n"
//                                " is the\r\n"
//                                "7\r\n"
//                                " response\r\n"
//                                "0\r\n"
//                                "\r\n";
// 	int	pos;
// 	std::string	sub_string;
// 	if ((pos = httpResponse.find("\r\n\r\n")) != std::string::npos)
// 		sub_string = httpResponse.substr(pos + 4, httpResponse.length() - pos - 8);

// 	int pos1;
// 	while ((pos1 = sub_string.find("\r\n", pos1)) != std::string::npos)
//     {
//         sub_string.replace(pos1, 2, "\n");
//         pos1 += 1;
//     }
// 	std::stringstream res(sub_string);
// 	std::string line;
// 	std::string body;

// 	while (getline(res, line, '\n'))
// 	{
// 		if (line[0] == '0' && line.length() == 1)
// 			break ;
// 		getline(res, line, '\n');
// 		body += line;
// 	}
// 	std::cout << body << std::endl;
// }

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <fstream>

int main()
{
	std::ifstream	mimieFile("mimes.txt");
	std::string		line;
	std::map<std::string, std::string> mimes;
	
	if (mimieFile.fail())
	{
		std::cout << "Error opening file" << std::endl;
		mimieFile.close();
	}
	getline(mimieFile, line);
	while (!(mimieFile.eof()))
	{
		
		std::stringstream	str(line);
		std::string			ext, type;
		getline(str, ext, ' ');
		getline(str, type);
		mimes[ext] = type;
		getline(mimieFile, line);
	}
	mimieFile.close();
	return (0);
}