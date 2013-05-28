#include "libelerrorpacket.h"

LibelErrorPacket::LibelErrorPacket(std::vector<unsigned char> input) throw (ZbCorruptedPacket) : ReceivePacket(input)
{
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (1, "NODE_RECEIVED_INVALID_PACKET_OF_TYPE_0_DONT_USE"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (2, "NODE_RECEIVED_INVALID_PACKET_OF_TYPE_2_ADD_NODE_RES"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (3, "NODE_RECEIVED_INVALID_PACKET_OF_TYPE_4_MASK_RES"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (4, "NODE_RECEIVED_INVALID_PACKET_OF_TYPE_6_CH_NODE_FREQ_RES"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (5, "NODE_RECEIVED_INVALID_PACKET_OF_TYPE_8_CH_SENS_FREQ_RES"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (6, "NODE_RECEIVED_INVALID_PACKET_OF_TYPE_10_IO_DATA"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (7, "NODE_RECEIVED_INVALID_PACKET_OF_TYPE_12_SEND_ERROR"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (8, "NODE_RECEIVED_AN_UNKNOWN_PACKET_TYPE"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (9, "NODE_RECEIVED_INVALID_MASK_IN_ADD_NODE_REQUEST"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (10, "MASK_RECEIVED_IN_ADD_NODE_REQUEST_WAS_REFUSED_BY_THE_PHYSICAL_MASK"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (11, "NODE_RECEIVED_INVALID_NEW_DEFAULT_SLEEP_TIME"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (12, "NODE_RECEIVED_INVALID_NEW_DEFAULT_SLEEP_TIME"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (13, "NODE_HAS_NO_LONGER_ACTIVE_SENSORS"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (14, "NODE_HAD_AN_ERROR_IN_SET_NEW_DIFFERENT_SLEEP_TIMES"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (15, "NODE_RECEIVED_A_MASK_THAT_DOESNT_MATCH_PHY_MASK_IN_IO_REQ"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (16, "NODE_HAD_AN_ERROR_IN_XBEEZB_CHANGE_SENSOR_FREQUENCIES"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (17, "SLEEP_TIME_EXCEEDED"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (18, "NODE_HAD_AN_ERROR_IN_PAQ_UTILS_IO_REQUEST_SENDING_SAMPLES"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (19, "NODE_HAD_AN_ERROR_IN_PAQ_UTILS_IO_REQUEST_MEASURE_SENSORS"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (20, "NODE_RECEIVED_AN_UNAUTHORIZED_REQUEST_OF_ADD_NODE_REQUEST"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (21, "NODE_RECEIVED_AN_UNAUTHORIZED_REQUEST_OF_MASK_REQUEST"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (22, "NODE_RECEIVED_AN_UNAUTHORIZED_REQUEST_OF_CH_SENS_FREQ_REQUEST"));
    LibelErrorPacket::errors.insert(std::pair<int, std::string> (23, "NODE_RECEIVED_AN_UNAUTHORIZED_REQUEST_OF_IO_REQUEST"));


    auto rfData = getRFData();
    if(rfData.at(0) != 0X0C)
    {
        std::cerr << "Tried to put a packed into a LibelIOPacket that was of the wrong type (see application ID != 0X0A)" << std::endl;
        throw ZbCorruptedPacket();
    }

    std::vector<unsigned char> data = getData();
    int error = data.at(0);
    auto foundErrorMsg = errors.find(error);
    if(foundErrorMsg == errors.end())
    {
        std::cerr << "Error in error message not known, int: " << (int) error;
        throw ZbCorruptedPacket();
    }
}

std::string LibelErrorPacket::getErrorMsg() const
{
    int error = getData().at(0);
    auto foundErrorMsg = errors.find(error);
    if(foundErrorMsg == errors.end())
    {
        std::cerr << "Error in error message not known, int: " << (int) error;
        return "Error in error message not known, int: " + (int) error;
    }
    else
    {
        return foundErrorMsg->second;
    }
}
