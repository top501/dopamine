/*************************************************************************
 * Research_pacs - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#define BOOST_TEST_MODULE ModuleExceptionPACS
#include <boost/test/unit_test.hpp>

#include "core/ExceptionPACS.h"

/*************************** TEST OK 01 *******************************/
/**
 * Nominal test case: Throw exception
 */
BOOST_AUTO_TEST_CASE(TEST_OK_01)
{
    try
    {
        throw research_pacs::ExceptionPACS("MyText");
    }
    catch (research_pacs::ExceptionPACS& exception)
    {
        BOOST_CHECK_EQUAL(exception.what(), "MyText");
    }
}