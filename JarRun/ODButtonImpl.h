// ODButtonImpl.h: interface for the CODButtonImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODBUTTONIMPL_H__97E2C002_D7F6_4DF1_85D2_EF418C0B7A19__INCLUDED_)
#define AFX_ODBUTTONIMPL_H__97E2C002_D7F6_4DF1_85D2_EF418C0B7A19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLCTRLS_H__
	#error ODButtonImpl.h requires atlctrls.h to be included first
#endif


class CODButtonImpl  : public CButton,
						public COwnerDraw<CODButtonImpl>
{
public:
	CODButtonImpl();
	virtual ~CODButtonImpl();

};

#endif // !defined(AFX_ODBUTTONIMPL_H__97E2C002_D7F6_4DF1_85D2_EF418C0B7A19__INCLUDED_)
