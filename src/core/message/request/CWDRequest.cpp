#include "core/message/request/CWDRequest.h"

using namespace FTP;

CWDRequest::CWDRequest(const std::string& directory) : Request(CWDRequest::CommandName), _directory(directory) {

}

const std::string& CWDRequest::getDirectory() const {
    return _directory;
}
