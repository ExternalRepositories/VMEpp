#ifndef V_PLUS_EXCEPTION_H
#define V_PLUS_EXCEPTION_H

#include <iostream>
#include <exception>
#include <string>

#include "CAENVMEtypes.h"


namespace vmeplus
{
    enum class VError_t {   vSuccess = cvSuccess,
                            vBusError = cvBusError,
                            vCommError = cvCommError,
                            vGenericError = cvGenericError,
                            vInvalidParam = cvInvalidParam,
                            vTimeoutError = cvTimeoutError,
                            vBadMaster = -10,
                            vOrphan = -11,
                            vBuffAllocFailed = -12,
                            vBadSlave = -13
                        };

    enum class Message_t {  INFO,
                            WARNING,
                            ERROR };

    class VException : public std::exception
    {
        protected :
            VError_t            fErrorCode;

            std::string         fMessage;
            std::string         fInfo;
            std::string         fHint;

        public :
            VException( VError_t errorCode, const std::string &addInfo ):
                fErrorCode( errorCode ),
                fMessage( "" ),
                fInfo( addInfo ),
                fHint( "N/A" )
            {
                switch( errorCode )
                {
                    case( VError_t::vSuccess ) :
                        fMessage = "Operation completed successfully"; break;
                    case( VError_t::vBusError ) :
                        fMessage = "VME bus error during the cycle";
                        fHint    = "Check your cable or reboot VME crate manually";
                        break;
                    case( VError_t::vCommError ) :
                        fMessage = "Communication error"; break;
                    case( VError_t::vGenericError ) :      
                        fMessage = "Unspecified error"; break;
                    case( VError_t::vInvalidParam ) :      
                        fMessage = "Invalid parameter"; break;
                    case( VError_t::vTimeoutError ) :      
                        fMessage = "Timeout error"; break;
                    case( VError_t::vBadMaster ) :
                        fMessage = "Requesting a bad controller";
                        fHint    = "Try to register this board to the correct master";
                        break;
                    case( VError_t::vOrphan ) :
                        fMessage = "The controller is invalid";
                        fHint = "Use VController::RegisterBoard( VBoard* ) method to register the board";
                        break;
                    case( VError_t::vBuffAllocFailed ) :
                        fMessage = "Failed to allocate memory for the readout buffer";
                        break;
                    case( VError_t::vBadSlave ) :
                        fMessage = "Bad master (base address, nullptr, etc.)";
                        break;
                }
            }

            virtual ~VException() throw() { };

            virtual const char* what() const throw()
            {
                return fMessage.c_str();
            }

            std::string GetErrorMessage() const throw()
            {
                return fMessage;
            }

            std::string GetInfo() const throw()
            {
                return fInfo;
            }

            std::string GetHint() const throw()
            {
                return fHint;
            }

            VError_t GetErrorCode() const { return fErrorCode; }
    };

    inline void PrintMessage( Message_t mType, const std::string &msg )
    {
        std::string prefix = "VME++";
        switch( mType )
        {
            case( Message_t::INFO ) :
                prefix = "\033[1;32mINFO :: " + prefix ;//Green colored text
                break;
            case( Message_t::WARNING ) :
                prefix = "\033[1;33mWARNING :: " + prefix;//Yellow colored text
                break;
            case( Message_t::ERROR ) :
                prefix = "\033[1;31mERROR :: " + prefix;//Red colored text
                break;
        }
        std::cerr << prefix << " : " << msg << "\033[0;0m" << std::endl;
    }
}
#endif//V_PLUS_EXCEPTION_H