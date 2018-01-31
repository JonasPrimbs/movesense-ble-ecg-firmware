#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/integration/port.h"
#include "whiteboard/builtinTypes/NoType.h"

namespace whiteboard
{

/** HTTP status codes used by Whiteboard API
*
* http://en.wikipedia.org/wiki/List_of_HTTP_status_codes
* http://www.restapitutorial.com/httpstatuscodes.html
*/
enum Result
{
    HTTP_CODE_INVALID = 0,      /** Indication of no result. Not part of the HTTP codes definition */

    /* INFORMATIONAL 1xx */
    HTTP_CODE_CONTINUE = 100,   /** HTTP 1.1: Continue with request, only partial content transmitted */
    HTTP_CODE_SWITCHING = 101,  /** HTTP 1.1: Switching protocols */
    HTTP_CODE_PROCESSING = 102, /** WEBDAV RFC 2518: Request started processing, but may take condireably long (>20s) */

    /* SUCCESS 2xx */
    HTTP_CODE_OK = 200,                /** HTTP 1.1: The request completed successfully */
    HTTP_CODE_CREATED = 201,           /** HTTP 1.1: The request has completed and a new resource was created */
    HTTP_CODE_ACCEPTED = 202,          /** HTTP 1.1: The request has been accepted and processing is continuing */
    HTTP_CODE_NOT_AUTHORITATIVE = 203, /** HTTP 1.1: The request has completed but content may be from another source */
    HTTP_CODE_NO_CONTENT = 204,        /** HTTP 1.1: The request has completed and there is no response to send */
    HTTP_CODE_RESET = 205,             /** HTTP 1.1: The request has completed with no content. Client must reset view */
    HTTP_CODE_PARTIAL = 206,           /** HTTP 1.1: The request has completed and is returning partial content */
    HTTP_CODE_MULTI_STATUS = 207,      /** WEBDAV RFC 4918 The message body is an XML message and can contain a number
                                           of separate response codes, depending on how many sub-requests were made */
    HTTP_CODE_ALREADY_REPORTED = 208,  /** WEBDAV RFC 5842 The members of a DAV binding have already been enumerated
                                           in a previous reply to this request, and are not being included again. */

    /* REDIRECTION 3xx */
    HTTP_CODE_MULTIPLE_CHOISES = 300,   /** HTTP 1.1: Multiple options for the resource that the client may follow */
    HTTP_CODE_MOVED_PERMANENTLY = 301,  /** HTTP 1.1: The requested URI has moved permanently to a new location */
    HTTP_CODE_MOVED_TEMPORARILY = 302,  /** HTTP 1.1: The URI has moved temporarily to a new location */
    HTTP_CODE_SEE_OTHER = 303,          /** HTTP 1.1: The requested URI can be found at another URI location */
    HTTP_CODE_NOT_MODIFIED = 304,       /** HTTP 1.1: The requested resource has not changed since the last request */
    HTTP_CODE_USE_PROXY = 305,          /** HTTP 1.1: The requested resource must be accessed via the location proxy */
    HTTP_CODE_TEMPORARY_REDIRECT = 307, /** HTTP 1.1: The request should be repeated at another URI location */

    /* CLIENT ERROR 4xx */
    HTTP_CODE_BAD_REQUEST = 400,            /** HTTP 1.1: The request is malformed */
    HTTP_CODE_UNAUTHORIZED = 401,           /** HTTP 1.1: Authentication for the request has failed */
    HTTP_CODE_PAYMENT_REQUIRED = 402,       /** HTTP 1.1: Reserved for future use */
    HTTP_CODE_FORBIDDEN = 403,              /** HTTP 1.1: The request was legal, but the server refuses to process */
    HTTP_CODE_NOT_FOUND = 404,              /** HTTP 1.1: The requested resource was not found */
    HTTP_CODE_BAD_METHOD = 405,             /** HTTP 1.1: The request HTTP method was not supported by the resource */
    HTTP_CODE_NOT_ACCEPTABLE = 406,         /** HTTP 1.1: The requested resource cannot generate the required content */
    HTTP_CODE_PROXY_AUTH_REQUIRED = 407,    /** HTTP 1.1: The client must first authenticate itself with the proxy. */
    HTTP_CODE_REQUEST_TIMEOUT = 408,        /** HTTP 1.1: The server timed out waiting for the request to complete */
    HTTP_CODE_CONFLICT = 409,               /** HTTP 1.1: The request had a conflict in the request headers and URI */
    HTTP_CODE_GONE = 410,                   /** HTTP 1.1: The requested resource is no longer available*/
    HTTP_CODE_LENGTH_REQUIRED = 411,        /** HTTP 1.1: The request did not specify a required content length*/
    HTTP_CODE_PRECOND_FAILED = 412,         /** HTTP 1.1: The server cannot satisfy one of the request preconditions */
    HTTP_CODE_REQUEST_TOO_LARGE = 413,      /** HTTP 1.1: The request is too large for the server to process */
    HTTP_CODE_REQUEST_URL_TOO_LARGE = 414,  /** HTTP 1.1: The request URI is too long for the server to process */
    HTTP_CODE_UNSUPPORTED_MEDIA_TYPE = 415, /** HTTP 1.1: The request media type is not supported by the server or resource */
    HTTP_CODE_RANGE_NOT_SATISFIABLE = 416,  /** HTTP 1.1: The request content range does not exist for the resource */
    HTTP_CODE_EXPECTATION_FAILED = 417,     /** HTTP 1.1: The server cannot satisfy the Expect header requirements */
    HTTP_CODE_AUTH_TIMEOUT = 419,           /** Not part of RFC: Previously valid authentication has timed out */
    HTTP_CODE_UNPROCESSABLE_ENTITY = 422,   /** WEBDAV RFC 4918: Request was well formed, but was unable to be processed
                                                due to semantic errors. */
    HTTP_CODE_LOCKED = 423,                 /** WEBDAV RFC 4918: The resource being accessed was locked. */
    HTTP_CODE_FAILED_DEPENDENCY = 424,      /** WEBDAV RFC 4918: The request failed due to failure of a previous request. */
    HTTP_CODE_UPGRADE_REQUIRED = 426,       /** RFC 2817: The client should upgrade the used protocol */
    HTTP_CODE_PRECONDITION_REQUIRED = 428,  /** RFC 6585: Request should be conditional to prevent lost update problem. */
    HTTP_CODE_TOO_MANY_REQUESTS = 429,      /** RFC 6585: The user has sent too many requests in given amount of time. */
    HTTP_CODE_REQUEST_HEADER_FIELDS_TOO_LARGE =
        431,                     /** RFC 6585: The server is unwilling to process the request because individual
                                     header field or all header fields collectively are too large. */
    HTTP_CODE_NO_RESPONSE = 444, /** NGINX: The connection was closed with no response to the client */

    /* SERVER ERRORS 5xx*/
    HTTP_CODE_INTERNAL_SERVER_ERROR = 500, /** HTTP 1.1: Server processing or configuration error. No response generated */
    HTTP_CODE_NOT_IMPLEMENTED = 501,       /** HTTP 1.1: The server does not recognize the request or method */
    HTTP_CODE_BAD_GATEWAY = 502,           /** HTTP 1.1: The server cannot act as a gateway for the given request */
    HTTP_CODE_SERVICE_UNAVAILABLE = 503,   /** HTTP 1.1: The server is currently unavailable or overloaded */
    HTTP_CODE_GATEWAY_TIMEOUT = 504,       /** HTTP 1.1: The server gateway timed out waiting for the upstream server */
    HTTP_CODE_BAD_VERSION = 505,           /** HTTP 1.1: The server does not support the HTTP protocol version */
    HTTP_CODE_INSUFFICIENT_STORAGE = 507   /** HTTP 1.1: The server has insufficient storage to complete the request */
};

/** Gets text representation of the error code
 *
 * @param resultCode Result code which text representation should be returned
 * @return Text formatted error message
 */
WB_API const char* getResultString(const Result resultCode);

/** Check if the given return value was successful (OK, ACCEPTED). */
inline bool RETURN_OK(const Result result)
{
    return result == wb::HTTP_CODE_OK || result == wb::HTTP_CODE_ACCEPTED;
}

/** Check if the given return value was successful (OK, ACCEPTED or CONTINUE). */
inline bool RETURN_OKC(const Result result)
{
    return result == wb::HTTP_CODE_OK || result == wb::HTTP_CODE_ACCEPTED || result == wb::HTTP_CODE_CONTINUE;
}

/** Check if the given return value was successful (OK, ACCEPTED or NO_CONTENT). */
inline bool RETURN_OKN(const Result result)
{
    return result == wb::HTTP_CODE_OK || result == wb::HTTP_CODE_ACCEPTED || result == wb::HTTP_CODE_NO_CONTENT;
}

/** @return true  if the result is NOT among 100 & 200 series of results, ie error */
inline bool IsErrorResult(const Result result)
{
    return (result != 0) && (result >= 300);
}

/** StronglyTypedResult class acts as wrapper class for type safe returnResult calls
 *
 * @tparam ValueType Type of the value
 * @tparam DEFAULT_RESULT_CODE Default result code to use in default constructor.
 *                             Needed for easier syntax of compile time instantiated WBRES generated results.
 */
template <typename ValueType = const NoType&, Result DEFAULT_RESULT_CODE = HTTP_CODE_INVALID>
class StronglyTypedResult FINAL
{
public:
    /** Type of the value */
    typedef ValueType Type;

    /** Constructor
     *
     * @param resultCode Result code of the StronglyTypedResult
     */
    inline StronglyTypedResult(Result resultCode = DEFAULT_RESULT_CODE)
        : mResultCode(resultCode)
    {
    }

    /** Copy constructor
     *
     * @tparam WHATEVER_DEFAULT Helper that allows assignment of a similar StronglyTypedResult but with different 
     *         default result code
     * @param rOther Another StronglyTypedResult instance to copy
     */
    template <Result WHATEVER_DEFAULT>
    inline StronglyTypedResult(const StronglyTypedResult<ValueType, WHATEVER_DEFAULT>& rOther)
        : mResultCode(static_cast<Result>(rOther))
    {
    }

    /** Assignment operator
    *
    * @tparam WHATEVER_DEFAULT Helper that allows assignment of a similar response but with different
    *         default result code
    * @param rOther Another response to copy
    * @return Reference to this StronglyTypedResult instance
    */
    template <Result WHATEVER_DEFAULT>
    inline const StronglyTypedResult& operator=(const StronglyTypedResult<ValueType, WHATEVER_DEFAULT>& rOther)
    {
        mResultCode = static_cast<Result>(rOther);
        return *this;
    }

    /** Cast operator to get the contained Result value
    *
    * @return Underlying result code
    */
    EXPLICIT inline operator Result() const
    {
        return mResultCode;
    }

    /** Helper function for doing compile time type checking of returnResult calls */
    static inline void typeCheck(ValueType)
    {
    }

private:
    /** Associated result code */
    Result mResultCode;
};

} // namespace whiteboard

