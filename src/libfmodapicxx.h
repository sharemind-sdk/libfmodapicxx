/*
 * This file is a part of the Sharemind framework.
 * Copyright (C) Cybernetica AS
 *
 * All rights are reserved. Reproduction in whole or part is prohibited
 * without the written consent of the copyright owner. The usage of this
 * code is subject to the appropriate license agreement.
 */

#ifndef SHAREMIND_LIBFMODAPICXX_LIBFMODAPICXX_H
#define SHAREMIND_LIBFMODAPICXX_LIBFMODAPICXX_H

#include <cassert>
#include <exception>
#include <new>
#include <sharemind/compiler-support/GccPR54526.h>
#include <sharemind/compiler-support/GccPR55015.h>
#include <sharemind/libfmodapi/libfmodapi.h>
#include <sharemind/libmodapi/libmodapicxx.h>


namespace sharemind {

/*******************************************************************************
  Some type aliases
*******************************************************************************/

using FacilityModuleApiError = ::SharemindFacilityModuleApiError;

class FacilityModuleApi;
class FacilityModule;


/*******************************************************************************
  Some helper macros
*******************************************************************************/

#define SHAREMIND_LIBFMODAPI_CXX_DEFINE_FACILITY_METHOD(ClassName,name,Name)\
    inline Facility const * find ## Name ## Facility( \
            char const * const signature) \
            const noexcept __attribute__ ((nonnull(2))) \
    { \
        assert(signature); \
        return ::Sharemind ## ClassName ## _find ## Name ## Facility( \
                m_c, \
                signature);\
    }
#define SHAREMIND_LIBFMODAPI_CXX_DEFINE_FACILITY_METHODS(ClassName) \
    SHAREMIND_LIBFMODAPI_CXX_DEFINE_FACILITY_METHOD(ClassName,module,Module)\
    SHAREMIND_LIBFMODAPI_CXX_DEFINE_FACILITY_METHOD(ClassName,pd,Pd) \
    SHAREMIND_LIBFMODAPI_CXX_DEFINE_FACILITY_METHOD(ClassName,pdpi,Pdpi)

#define SHAREMIND_LIBFMODAPI_CXX_DEFINE_CPTR_GETTERS(ClassName) \
    inline ::Sharemind ## ClassName * cPtr() noexcept { return m_c; } \
    inline ::Sharemind ## ClassName const * cPtr() const noexcept { return m_c;}


/*******************************************************************************
  Details
*******************************************************************************/

namespace Detail {
namespace libfmodapi {

inline FacilityModuleApiError allocThrow(FacilityModuleApiError const e) {
    if (e == ::SHAREMIND_FACILITY_MODULE_API_OUT_OF_MEMORY)
        throw ::std::bad_alloc{};
    return e;
}

} /* namespace libfmodapi { */
} /* namespace Detail { */


/*******************************************************************************
  FacilityModuleApiError
*******************************************************************************/

inline char const * FacilityModuleApiError_toString(
        FacilityModuleApiError const e) noexcept
{ return ::SharemindFacilityModuleApiError_toString(e); }



/*******************************************************************************
  *::Exception
*******************************************************************************/

class FacilityModuleApiExceptionBase: public ::std::exception {

public: /* Methods: */

    inline FacilityModuleApiExceptionBase(
            FacilityModuleApiError const errorCode,
            char const * const errorStr)
        : m_errorCode{(assert(errorCode != ::SHAREMIND_FACILITY_MODULE_API_OK),
                       errorCode)}
        , m_errorStr{errorStr
                     ? errorStr
                     : FacilityModuleApiError_toString(errorCode)}
    {}

    inline FacilityModuleApiError code() const noexcept { return m_errorCode; }
    inline char const * what() const noexcept override { return m_errorStr; }

private: /* Fields: */

    FacilityModuleApiError const m_errorCode;
    char const * const m_errorStr;

}; /* class ModuleApiExceptionBase { */

#define SHAREMIND_LIBFMODAPI_CXX_DEFINE_EXCEPTION(ClassName) \
    class Exception: public FacilityModuleApiExceptionBase { \
    public: /* Methods: */ \
        inline Exception(::Sharemind ## ClassName const & c) \
            : FacilityModuleApiExceptionBase{ \
                      Detail::libfmodapi::allocThrow( \
                              ::Sharemind ## ClassName ## _lastError(&c)), \
                      ::Sharemind ## ClassName ## _lastErrorString(&c)} \
        {} \
        inline Exception(ClassName const & c) \
            : Exception{*(c.cPtr())} \
        {} \
        inline Exception(FacilityModuleApiError const error, \
                         char const * const errorStr = nullptr) \
            : FacilityModuleApiExceptionBase{error, errorStr} \
        {} \
        inline Exception(FacilityModuleApiError const error, \
                         ::Sharemind ## ClassName const & c) \
            : FacilityModuleApiExceptionBase{ \
                    error, \
                    ::Sharemind ## ClassName ## _lastErrorString(&c)} \
        {} \
        inline Exception(FacilityModuleApiError const error, \
                         ClassName const & c) \
            : Exception{error, *(c.cPtr())} \
        {} \
    }

/*******************************************************************************
  FacilityModule
*******************************************************************************/

class FacilityModule {

public: /* Types: */

    SHAREMIND_LIBFMODAPI_CXX_DEFINE_EXCEPTION(FacilityModule);

public: /* Methods: */

    FacilityModule() = delete;
    FacilityModule(FacilityModule &&) = delete;
    FacilityModule(FacilityModule const &) = delete;
    FacilityModule & operator=(FacilityModule &&) = delete;
    FacilityModule & operator=(FacilityModule const &) = delete;

    FacilityModule(FacilityModuleApi & moduleApi,
           char const * const filename,
           char const * const configuration) __attribute__ ((nonnull(3)));

    virtual inline ~FacilityModule() noexcept {
        if (m_c) {
            if (::SharemindFacilityModule_tag(m_c) == this)
                ::SharemindFacilityModule_releaseTag(m_c);
            ::SharemindFacilityModule_free(m_c);
        }
    }

    SHAREMIND_LIBFMODAPI_CXX_DEFINE_CPTR_GETTERS(FacilityModule)

    inline FacilityModuleApi * facilityModuleApi() const noexcept {
        ::SharemindFacilityModuleApi * const cfmodapi =
                ::SharemindFacilityModule_facilityModuleApi(m_c);
        assert(cfmodapi);
        FacilityModuleApi * const fmodapi =
                static_cast<FacilityModuleApi *>(
                    ::SharemindFacilityModuleApi_tag(cfmodapi));
        assert(fmodapi);
        return fmodapi;
    }
    SHAREMIND_LIBFMODAPI_CXX_DEFINE_FACILITY_METHODS(FacilityModule)

    inline void init() {
        FacilityModuleApiError const r = ::SharemindFacilityModule_init(m_c);
        if (r != ::SHAREMIND_FACILITY_MODULE_API_OK)
            throw Exception{r, *this};
    }

    inline void deinit() noexcept { ::SharemindFacilityModule_deinit(m_c); }

    inline bool isInitialized() const noexcept
    { return ::SharemindFacilityModule_isInitialized(m_c); }

    inline char const * filename() const noexcept
    { return ::SharemindFacilityModule_filename(m_c); }

    inline char const * name() const noexcept
    { return ::SharemindFacilityModule_name(m_c); }

    inline char const * conf() const noexcept
    { return ::SharemindFacilityModule_conf(m_c); }

    inline uint32_t apiVersionInUse() const noexcept
    { return ::SharemindFacilityModule_apiVersionInUse(m_c); }

private: /* Fields: */

    ::SharemindFacilityModule * m_c;

}; /* class FacilityModule { */

/*******************************************************************************
  FacilityModuleApi
*******************************************************************************/

class FacilityModuleApi {

    friend FacilityModule::FacilityModule(FacilityModuleApi &,
                                          char const * const,
                                          char const * const);

public: /* Types: */

    SHAREMIND_LIBFMODAPI_CXX_DEFINE_EXCEPTION(FacilityModuleApi);

public: /* Methods: */

    FacilityModuleApi(FacilityModuleApi &&) = delete;
    FacilityModuleApi(FacilityModuleApi const &) = delete;
    FacilityModuleApi & operator=(FacilityModuleApi &&) = delete;
    FacilityModuleApi & operator=(FacilityModuleApi const &) = delete;

    inline FacilityModuleApi()
        : m_c{[]{
                  FacilityModuleApiError error;
                  char const * errorStr;
                  ::SharemindFacilityModuleApi * const fmodapi =
                          ::SharemindFacilityModuleApi_new(&error,
                                                           &errorStr);
                  if (fmodapi)
                      return fmodapi;
                  throw Exception{Detail::libfmodapi::allocThrow(error),
                                  errorStr};
              }()}
    {
        #define SHAREMIND_LIBFMODAPI_CXX_MODULEAPI_L1 \
            (void * m) noexcept { \
                FacilityModuleApi * const moduleApi = \
                        static_cast<FacilityModuleApi *>(m); \
                moduleApi->m_c = nullptr; \
                delete moduleApi; \
            }
        #if SHAREMIND_GCCPR55015
        struct F { static void f SHAREMIND_LIBFMODAPI_CXX_MODULEAPI_L1 };
        #endif
        ::SharemindFacilityModuleApi_setTagWithDestructor(
                    m_c,
                    this,
                    #if SHAREMIND_GCCPR55015
                    &F::f
                    #else
                    []SHAREMIND_LIBFMODAPI_CXX_MODULEAPI_L1
                    #endif
                    );
        #undef SHAREMIND_LIBFMODAPI_CXX_MODULEAPI_L1
    }

    virtual inline ~FacilityModuleApi() noexcept {
        if (m_c) {
            if (::SharemindFacilityModuleApi_tag(m_c) == this)
                ::SharemindFacilityModuleApi_releaseTag(m_c);
            ::SharemindFacilityModuleApi_free(m_c);
        }
    }

    SHAREMIND_LIBFMODAPI_CXX_DEFINE_CPTR_GETTERS(FacilityModuleApi)
    SHAREMIND_LIBFMODAPI_CXX_DEFINE_FACILITY_METHODS(
            FacilityModuleApi)

private: /* Methods: */

    ::SharemindFacilityModule & newModule(char const * const filename,
                                          char const * const configuration)
            __attribute__((nonnull(2)))
    {
        assert(filename);
        ::SharemindFacilityModule * const m =
                ::SharemindFacilityModuleApi_newModule(m_c,
                                                       filename,
                                                       configuration);
        if (m)
            return *m;
        throw Exception{*m_c};
    }

private: /* Fields: */

    ::SharemindFacilityModuleApi * m_c;

}; /* class FacilityModuleApi { */


/*******************************************************************************
  FacilityModule methods
*******************************************************************************/

inline FacilityModule::FacilityModule(FacilityModuleApi & moduleApi,
                                      char const * const filename,
                                      char const * const configuration)
    : m_c{&moduleApi.newModule(filename, configuration)}
{
    try {
        #define SHAREMIND_LIBFMODAPI_CXX_MODULE_L1 \
            (void * m) noexcept { \
                FacilityModule * const module = \
                        static_cast<FacilityModule *>(m); \
                module->m_c = nullptr; \
                delete module; \
            }
        #if SHAREMIND_GCCPR55015
        struct F { static void f SHAREMIND_LIBFMODAPI_CXX_MODULE_L1 };
        #endif
        ::SharemindFacilityModule_setTagWithDestructor(
                    m_c,
                    this,
                    #if SHAREMIND_GCCPR55015
                    &F::f
                    #else
                    []SHAREMIND_LIBFMODAPI_CXX_MODULE_L1
                    #endif
                    );
        #undef SHAREMIND_LIBFMODAPI_CXX_MODULE_L1
    } catch (...) {
        ::SharemindFacilityModule_free(m_c);
        throw;
    }
}

} /* namespace sharemind { */


/*******************************************************************************
  Undefine helper macros
*******************************************************************************/

#undef SHAREMIND_LIBFMODAPI_CXX_DEFINE_EXCEPTION
#undef SHAREMIND_LIBFMODAPI_CXX_DEFINE_CPTR_GETTERS
#undef SHAREMIND_LIBFMODAPI_CXX_DEFINE_PARENT_GETTER
#undef SHAREMIND_LIBFMODAPI_CXX_DEFINE_FACILITY_METHODS
#undef SHAREMIND_LIBFMODAPI_CXX_DEFINE_FACILITY_METHOD

#endif /* SHAREMIND_LIBFMODAPICXX_LIBFMODAPICXX_H */
