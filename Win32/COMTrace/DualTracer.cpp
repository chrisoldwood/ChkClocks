////////////////////////////////////////////////////////////////////////////////
//! \file   DualTracer.cpp
//! \brief  The DualTracer class definition.
//! \author Chris Oldwood

#include "COMTrace.hpp"
#include "DualTracer.hpp"
#include <atlconv.h>

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

DualTracer::DualTracer()
	: COMTraceObject<IDualInterface>("DualTracer")
	, COM::IDispatchImpl<DualTracer>(IID_IDualInterface)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

DualTracer::~DualTracer()
{
}

////////////////////////////////////////////////////////////////////////////////
//! The test method.

HRESULT COMCALL DualTracer::TestMethod(BSTR bstrInput, BSTR* pbstrOutput)
{
	LOG_ENTRY("DualTracer::TestMethod()");

	HRESULT hr = S_OK;

	try
	{
		// Check output parameters.
		if (pbstrOutput == nullptr)
			throw WCL::ComException(E_POINTER, "pbstrOutput is NULL");

		// Reverse the input string.
		*pbstrOutput = _wcsrev(::SysAllocString(bstrInput));
	}
	COM_CATCH(hr)

	LOG_EXIT("HRESULT=0x%08X [%s]", hr, CStrCvt::FormatError(hr));

	return hr;
}

////////////////////////////////////////////////////////////////////////////////
//! Get the number of type information interfaces supported.

HRESULT COMCALL DualTracer::GetTypeInfoCount(UINT* pnInfo)
{
	LOG_ENTRY("DualTracer::GetTypeInfoCount(UINT*)");

	HRESULT hr = S_OK;

	try
	{
		// Check output parameters.
		if (pnInfo == nullptr)
			throw WCL::ComException(E_POINTER, "pnInfo is NULL");

		*pnInfo = 1;
	}
	COM_CATCH(hr)

	LOG_EXIT("HRESULT=0x%08X [%s]", hr, CStrCvt::FormatError(hr));

	return hr;
}

////////////////////////////////////////////////////////////////////////////////
//! Get the type information for an object.

HRESULT COMCALL DualTracer::GetTypeInfo(UINT nInfo, LCID dwLCID, ITypeInfo** ppTypeInfo)
{
	LOG_ENTRY("DualTracer::GetTypeInfoCount(UINT, dwLCID)");
	LOG_PARAM("UINT=%u", nInfo);
	LOG_PARAM("dwLCID=%u", dwLCID);

	HRESULT hr = S_OK;

	try
	{
		hr = COM::IDispatchImpl<DualTracer>::GetTypeInfo(nInfo, dwLCID, ppTypeInfo);
	}
	COM_CATCH(hr)

	LOG_EXIT("HRESULT=0x%08X [%s]", hr, CStrCvt::FormatError(hr));

	return hr;
}

////////////////////////////////////////////////////////////////////////////////
//! Map a number of names to their dispatch IDs.

HRESULT COMCALL DualTracer::GetIDsOfNames(REFIID rIID, LPOLESTR* aszNames, UINT nNames, LCID dwLCID, DISPID* alMemberIDs)
{
	USES_CONVERSION;

	LOG_ENTRY("DualTracer::GetIDsOfNames(REFIID, LPOLESTR*, UINT, LCID, DISPID*)");
	LOG_PARAM("UINT=%u", nNames);

	for (size_t i = 0; i < nNames; ++i)
		LOG_PARAM("LPOLESTR*[%d]=%s", i, W2T(aszNames[i]));

	HRESULT hr = S_OK;

	try
	{
		hr = COM::IDispatchImpl<DualTracer>::GetIDsOfNames(rIID, aszNames, nNames, dwLCID, alMemberIDs);
	}
	COM_CATCH(hr)

	LOG_EXIT("HRESULT=0x%08X [%s]", hr, CStrCvt::FormatError(hr));

	return hr;
}

////////////////////////////////////////////////////////////////////////////////
//! Invoke a method or access a property.

HRESULT COMCALL DualTracer::Invoke(DISPID lMemberID, REFIID rIID, LCID dwLCID, WORD wFlags, DISPPARAMS* pParams, VARIANT* pResult, EXCEPINFO* pExcepInfo, UINT* pnArgError)
{
	LOG_ENTRY("DualTracer::Invoke(DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT*)");
	LOG_PARAM("DISPID=%d", lMemberID);
	LOG_PARAM("WORD=0x%08X", static_cast<DWORD>(wFlags));
	LOG_PARAM("DISPPARAMS*=%u %u", pParams->cArgs, pParams->cNamedArgs);

	HRESULT hr = S_OK;

	try
	{
		hr = COM::IDispatchImpl<DualTracer>::Invoke(lMemberID, rIID, dwLCID, wFlags, pParams, pResult, pExcepInfo, pnArgError);
	}
	COM_CATCH(hr)

	LOG_EXIT("HRESULT=0x%08X [%s]", hr, CStrCvt::FormatError(hr));

	return hr;
}
