/*************************************************************************
 * dopamine - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#define BOOST_TEST_MODULE ModuleDataSetToBSON
#include <boost/test/unit_test.hpp>

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dctk.h>

#include "ConverterBSON/Dataset/DataSetToBSON.h"
#include "ConverterBSON/Dataset/TagMatch.h"

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Constructor / Destructor
 */
BOOST_AUTO_TEST_CASE(Constructor)
{
    dopamine::converterBSON::DataSetToBSON * datasettobson =
            new dopamine::converterBSON::DataSetToBSON();

    // Object build
    BOOST_CHECK_EQUAL(datasettobson != NULL, true);
    
    // Default value
    BOOST_CHECK_EQUAL(datasettobson->get_specific_character_set() == "", true);
    BOOST_CHECK_EQUAL(datasettobson->get_default_filter() == 
                      dopamine::converterBSON::DataSetToBSON::FilterAction::INCLUDE, true);
    BOOST_CHECK_EQUAL(datasettobson->get_filters().size() == 0, true);

    delete datasettobson;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Getter and Setter
 */
BOOST_AUTO_TEST_CASE(GetterAndSetter)
{
    dopamine::converterBSON::DataSetToBSON datasettobson;

    // set_specific_character_set
    datasettobson.set_specific_character_set("ISO_IR 192");
    // Default value
    BOOST_CHECK_EQUAL(datasettobson.get_specific_character_set() == "ISO_IR 192", true);

    // set_default_filter
    datasettobson.set_default_filter(dopamine::converterBSON::DataSetToBSON::FilterAction::EXCLUDE);
    // check value
    BOOST_CHECK_EQUAL(datasettobson.get_default_filter() ==
                      dopamine::converterBSON::DataSetToBSON::FilterAction::EXCLUDE, true);

    // Set Filter
    std::vector<dopamine::converterBSON::DataSetToBSON::Filter> filters;
    filters.push_back(std::make_pair(dopamine::converterBSON::TagMatch::New(DCM_PatientName),
                      dopamine::converterBSON::DataSetToBSON::FilterAction::INCLUDE));
                      
    datasettobson.set_filters(filters);
    
    std::vector<dopamine::converterBSON::DataSetToBSON::Filter> const getfilters =
        datasettobson.get_filters();
    // check value
    BOOST_CHECK_EQUAL(getfilters.size(), 1);
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR AE
 */
BOOST_AUTO_TEST_CASE(ConversionAE)
{
    DcmDataset* dataset = new DcmDataset();
    dataset->putAndInsertOFStringArray(DCM_RetrieveAETitle, "value01\\value02");

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement element = it.next();
    mongo::BSONObj object = element.Obj();
    BOOST_CHECK_EQUAL(element.fieldName(), "00080054");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "AE");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].String(), "value01");
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].String(), "value02");

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR AS
 */
BOOST_AUTO_TEST_CASE(ConversionAS)
{
    DcmDataset* dataset = new DcmDataset();
    dataset->putAndInsertOFStringArray(DCM_PatientAge, "value01\\value02");

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement element = it.next();
    mongo::BSONObj object = element.Obj();
    BOOST_CHECK_EQUAL(element.fieldName(), "00101010");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "AS");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].String(), "value01");
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].String(), "value02");

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR AT
 */
BOOST_AUTO_TEST_CASE(ConversionAT)
{
    DcmDataset* dataset = new DcmDataset();
    DcmElement * element = NULL;
    dataset->insertEmptyElement(DCM_DimensionIndexPointer);
    dataset->findAndGetElement(DCM_DimensionIndexPointer, element);
    std::vector<Uint16> vectoruint16 = { DCM_PatientName.getGroup() , DCM_PatientName.getElement() };
    element->putUint16Array(&vectoruint16[0], 1);

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement elementbson = it.next();
    mongo::BSONObj object = elementbson.Obj();
    BOOST_CHECK_EQUAL(elementbson.fieldName(), "00209165");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "AT");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 1);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].String(), "(0010,0010)");

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR CS
 */
BOOST_AUTO_TEST_CASE(ConversionCS)
{
    DcmDataset* dataset = new DcmDataset();
    dataset->putAndInsertOFStringArray(DCM_Modality, "value01\\value02");

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement element = it.next();
    mongo::BSONObj object = element.Obj();
    BOOST_CHECK_EQUAL(element.fieldName(), "00080060");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "CS");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].String(), "value01");
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].String(), "value02");

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR DA
 */
BOOST_AUTO_TEST_CASE(ConversionDA)
{
    DcmDataset* dataset = new DcmDataset();
    dataset->putAndInsertOFStringArray(DCM_PatientBirthDate, "value01\\value02");

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement element = it.next();
    mongo::BSONObj object = element.Obj();
    BOOST_CHECK_EQUAL(element.fieldName(), "00100030");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "DA");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].String(), "value01");
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].String(), "value02");

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR DS
 */
BOOST_AUTO_TEST_CASE(ConversionDS)
{
    DcmDataset* dataset = new DcmDataset();
    dataset->putAndInsertOFStringArray(DCM_PatientWeight, "61.5\\62.5");

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement element = it.next();
    mongo::BSONObj object = element.Obj();
    BOOST_CHECK_EQUAL(element.fieldName(), "00101030");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "DS");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].Double(), 61.5);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].Double(), 62.5);

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR DT
 */
BOOST_AUTO_TEST_CASE(ConversionDT)
{
    DcmDataset* dataset = new DcmDataset();
    dataset->putAndInsertOFStringArray(DCM_FrameAcquisitionDateTime, "value01\\value02");

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement element = it.next();
    mongo::BSONObj object = element.Obj();
    BOOST_CHECK_EQUAL(element.fieldName(), "00189074");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "DT");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].String(), "value01");
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].String(), "value02");

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR FD
 */
BOOST_AUTO_TEST_CASE(ConversionFD)
{
    DcmDataset* dataset = new DcmDataset();
    dataset->insertEmptyElement(DCM_PupilSize);
    DcmElement * element = NULL;
    dataset->findAndGetElement(DCM_PupilSize, element);
    std::vector<Float64> vectorfloat64 = {42.5, 43.6};
    element->putFloat64Array(&vectorfloat64[0], 2);

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement elementbson = it.next();
    mongo::BSONObj object = elementbson.Obj();
    BOOST_CHECK_EQUAL(elementbson.fieldName(), "00460044");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "FD");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].Double(), 42.5);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].Double(), 43.6);

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR FL
 */
BOOST_AUTO_TEST_CASE(ConversionFL)
{
    DcmDataset* dataset = new DcmDataset();
    DcmElement * element = NULL;
    dataset->insertEmptyElement(DCM_RecommendedDisplayFrameRateInFloat);
    dataset->findAndGetElement(DCM_RecommendedDisplayFrameRateInFloat, element);
    std::vector<Float32> vectorfloat32 = {42.5, 43.6};
    element->putFloat32Array(&vectorfloat32[0], 2);

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement elementbson = it.next();
    mongo::BSONObj object = elementbson.Obj();
    BOOST_CHECK_EQUAL(elementbson.fieldName(), "00089459");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "FL");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);
    BOOST_CHECK_CLOSE(object.getField("Value").Array()[0].Double(), 42.5, 0.001);
    BOOST_CHECK_CLOSE(object.getField("Value").Array()[1].Double(), 43.6, 0.001);

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR IS
 */
BOOST_AUTO_TEST_CASE(ConversionIS)
{
    DcmDataset* dataset = new DcmDataset();
    dataset->putAndInsertOFStringArray(DCM_StageNumber, "12\\13");

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement element = it.next();
    mongo::BSONObj object = element.Obj();
    BOOST_CHECK_EQUAL(element.fieldName(), "00082122");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "IS");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].Int(), 12);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].Int(), 13);

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR LO
 */
BOOST_AUTO_TEST_CASE(ConversionLO)
{
    DcmDataset* dataset = new DcmDataset();
    dataset->putAndInsertOFStringArray(DCM_Manufacturer, "value01\\value02");

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement element = it.next();
    mongo::BSONObj object = element.Obj();
    BOOST_CHECK_EQUAL(element.fieldName(), "00080070");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "LO");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].String(), "value01");
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].String(), "value02");

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR LT
 */
BOOST_AUTO_TEST_CASE(ConversionLT)
{
    DcmDataset* dataset = new DcmDataset();
    // Be carefull: putAndInsertOFStringArray for LT add only 1 value !!!
    dataset->putAndInsertOFStringArray(DCM_AdditionalPatientHistory, "value01\\value02");

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement element = it.next();
    mongo::BSONObj object = element.Obj();
    BOOST_CHECK_EQUAL(element.fieldName(), "001021b0");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "LT");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 1);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].String(), "value01\\value02");
    //BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].String(), "value02");

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}


/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR OB
 */
BOOST_AUTO_TEST_CASE(ConversionOB)
{
    std::string const value = "azertyuiopqsdfghjklmwxcvbn123456";
    DcmDataset* dataset = new DcmDataset();
    OFCondition condition = dataset->putAndInsertUint8Array(DCM_ICCProfile,
                                                            reinterpret_cast<Uint8 const *>(value.c_str()),
                                                            32);

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement element = it.next();
    mongo::BSONObj object = element.Obj();
    BOOST_CHECK_EQUAL(element.fieldName(), "00282000");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "OB");
    BOOST_CHECK_EQUAL(object.getField("InlineBinary").type() == mongo::BSONType::BinData, true);

    int size=0;
    char const * begin = object.getField("InlineBinary").binDataClean(size);
    BOOST_CHECK_EQUAL(std::string(begin, size), value);

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR OF
 */
BOOST_AUTO_TEST_CASE(ConversionOF)
{
    std::string const value = "azertyuiopqsdfghjklmwxcvbn123456";
    DcmDataset* dataset = new DcmDataset();
    DcmElement * element = NULL;
    dataset->insertEmptyElement(DCM_VectorGridData);
    dataset->findAndGetElement(DCM_VectorGridData, element);
    element->putFloat32Array(reinterpret_cast<Float32 const *>(value.c_str()), 8);

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement elementbson = it.next();
    mongo::BSONObj object = elementbson.Obj();
    BOOST_CHECK_EQUAL(elementbson.fieldName(), "00640009");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "OF");
    BOOST_CHECK_EQUAL(object.getField("InlineBinary").type() == mongo::BSONType::BinData, true);

    int size=0;
    char const * begin = object.getField("InlineBinary").binDataClean(size);
    BOOST_CHECK_EQUAL(std::string(begin, size), value);

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR OW
 */
BOOST_AUTO_TEST_CASE(ConversionOW)
{
    std::string const value = "azertyuiopqsdfghjklmwxcvbn123456";
    DcmDataset* dataset = new DcmDataset();
    DcmElement * element = NULL;
    dataset->insertEmptyElement(DCM_TrianglePointIndexList);
    dataset->findAndGetElement(DCM_TrianglePointIndexList, element);
    OFCondition condition = element->putUint16Array(reinterpret_cast<Uint16 const *>(value.c_str()),
                                                    16);

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement elementbson = it.next();
    mongo::BSONObj object = elementbson.Obj();
    BOOST_CHECK_EQUAL(elementbson.fieldName(), "00660023");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "OW");
    BOOST_CHECK_EQUAL(object.getField("InlineBinary").type() == mongo::BSONType::BinData, true);

    int size=0;
    char const * begin = object.getField("InlineBinary").binDataClean(size);
    BOOST_CHECK_EQUAL(std::string(begin, size), value);

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR PN
 */
BOOST_AUTO_TEST_CASE(ConversionPN)
{
    DcmDataset* dataset = new DcmDataset();
    dataset->putAndInsertOFStringArray(DCM_PatientName, "Doe^John^Wallas^Rev.^Chief Executive Officer\\Smith^Jane^Scarlett^Ms.^Goddess");

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement element = it.next();
    mongo::BSONObj object = element.Obj();
    BOOST_CHECK_EQUAL(element.fieldName(), "00100010");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "PN");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);

    mongo::BSONObj component_name = object.getField("Value").Array()[0].Obj();
    BOOST_CHECK_EQUAL(component_name.getField("Alphabetic").String(), "Doe^John^Wallas^Rev.^Chief Executive Officer");
    component_name = object.getField("Value").Array()[1].Obj();
    BOOST_CHECK_EQUAL(component_name.getField("Alphabetic").String(), "Smith^Jane^Scarlett^Ms.^Goddess");

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR SH
 */
BOOST_AUTO_TEST_CASE(ConversionSH)
{
    DcmDataset* dataset = new DcmDataset();
    dataset->putAndInsertOFStringArray(DCM_EthnicGroup, "value01\\value02");

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement element = it.next();
    mongo::BSONObj object = element.Obj();
    BOOST_CHECK_EQUAL(element.fieldName(), "00102160");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "SH");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].String(), "value01");
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].String(), "value02");

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR SL
 */
BOOST_AUTO_TEST_CASE(ConversionSL)
{
    DcmDataset* dataset = new DcmDataset();
    DcmElement * element = NULL;
    dataset->insertEmptyElement(DCM_ReferencePixelX0);
    dataset->findAndGetElement(DCM_ReferencePixelX0, element);
    std::vector<Sint32> vectorsint32 = {10, 11};
    element->putSint32Array(&vectorsint32[0], 2);

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement elementbson = it.next();
    mongo::BSONObj object = elementbson.Obj();
    BOOST_CHECK_EQUAL(elementbson.fieldName(), "00186020");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "SL");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].Int(), 10);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].Int(), 11);

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR SQ
 */
BOOST_AUTO_TEST_CASE(ConversionSQ)
{
    DcmDataset* dataset = new DcmDataset();
    DcmItem* item = new DcmItem(DCM_OtherPatientIDsSequence);
    item->putAndInsertOFStringArray(DCM_PatientID, "123");
    dataset->insertSequenceItem(DCM_OtherPatientIDsSequence, item);
    item = new DcmItem(DCM_OtherPatientIDsSequence);
    item->putAndInsertOFStringArray(DCM_PatientID, "123");
    dataset->insertSequenceItem(DCM_OtherPatientIDsSequence, item, 1);

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement elementbson = it.next();
    mongo::BSONObj object = elementbson.Obj();
    BOOST_CHECK_EQUAL(elementbson.fieldName(), "00101002");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "SQ");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);

    mongo::BSONObj object_item = object.getField("Value").Array()[0].Obj();
    mongo::BSONObj::iterator it_item = object_item.begin();
    mongo::BSONElement elementbson_item = it_item.next();
    mongo::BSONObj object_subitem = elementbson_item.Obj();
    BOOST_CHECK_EQUAL(elementbson_item.fieldName(), "00100020");
    BOOST_CHECK_EQUAL(object_subitem.getField("vr").String(), "LO");
    BOOST_CHECK_EQUAL(object_subitem.getField("Value").Array().size(), 1);
    BOOST_CHECK_EQUAL(object_subitem.getField("Value").Array()[0].String(), "123");

    object_item = object.getField("Value").Array()[1].Obj();
    it_item = object_item.begin();
    elementbson_item = it_item.next();
    object_subitem = elementbson_item.Obj();
    BOOST_CHECK_EQUAL(elementbson_item.fieldName(), "00100020");
    BOOST_CHECK_EQUAL(object_subitem.getField("vr").String(), "LO");
    BOOST_CHECK_EQUAL(object_subitem.getField("Value").Array().size(), 1);
    BOOST_CHECK_EQUAL(object_subitem.getField("Value").Array()[0].String(), "123");

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR SS
 */
BOOST_AUTO_TEST_CASE(ConversionSS)
{
    DcmDataset* dataset = new DcmDataset();
    DcmElement * element = NULL;
    dataset->insertEmptyElement(DCM_TagAngleSecondAxis);
    dataset->findAndGetElement(DCM_TagAngleSecondAxis, element);
    std::vector<Sint16> vectorsint16 = {10, 11};
    element->putSint16Array(&vectorsint16[0], 2);

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement elementbson = it.next();
    mongo::BSONObj object = elementbson.Obj();
    BOOST_CHECK_EQUAL(elementbson.fieldName(), "00189219");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "SS");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].Int(), 10);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].Int(), 11);

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR ST
 */
BOOST_AUTO_TEST_CASE(ConversionST)
{
    DcmDataset* dataset = new DcmDataset();
    // Be carefull: putAndInsertOFStringArray for ST add only 1 value !!!
    dataset->putAndInsertOFStringArray(DCM_InstitutionAddress, "value01\\value02");

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement element = it.next();
    mongo::BSONObj object = element.Obj();
    BOOST_CHECK_EQUAL(element.fieldName(), "00080081");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "ST");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 1);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].String(), "value01\\value02");
    //BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].String(), "value02");

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR TM
 */
BOOST_AUTO_TEST_CASE(ConversionTM)
{
    DcmDataset* dataset = new DcmDataset();
    dataset->putAndInsertOFStringArray(DCM_InstanceCreationTime, "value01\\value02");

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement element = it.next();
    mongo::BSONObj object = element.Obj();
    BOOST_CHECK_EQUAL(element.fieldName(), "00080013");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "TM");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].String(), "value01");
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].String(), "value02");

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR UI
 */
BOOST_AUTO_TEST_CASE(ConversionUI)
{
    DcmDataset* dataset = new DcmDataset();
    dataset->putAndInsertOFStringArray(DCM_SOPClassUID, "value01\\value02");

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement element = it.next();
    mongo::BSONObj object = element.Obj();
    BOOST_CHECK_EQUAL(element.fieldName(), "00080016");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "UI");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].String(), "value01");
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].String(), "value02");

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR UL
 */
BOOST_AUTO_TEST_CASE(ConversionUL)
{
    DcmDataset* dataset = new DcmDataset();
    DcmElement * element = NULL;
    dataset->insertEmptyElement(DCM_SimpleFrameList);
    dataset->findAndGetElement(DCM_SimpleFrameList, element);
    std::vector<Uint32> vectoruint32 = {10, 11};
    element->putUint32Array(&vectoruint32[0], 2);

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement elementbson = it.next();
    mongo::BSONObj object = elementbson.Obj();
    BOOST_CHECK_EQUAL(elementbson.fieldName(), "00081161");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "UL");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].Int(), 10);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].Int(), 11);

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR UN
 */
BOOST_AUTO_TEST_CASE(ConversionUN)
{
    // TODO
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR US
 */
BOOST_AUTO_TEST_CASE(ConversionUS)
{
    DcmDataset* dataset = new DcmDataset();
    DcmElement * element = NULL;
    dataset->insertEmptyElement(DCM_FailureReason);
    dataset->findAndGetElement(DCM_FailureReason, element);
    std::vector<Uint16> vectoruint16 = {10, 11};
    element->putUint16Array(&vectoruint16[0], 2);

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement elementbson = it.next();
    mongo::BSONObj object = elementbson.Obj();
    BOOST_CHECK_EQUAL(elementbson.fieldName(), "00081197");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "US");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 2);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].Int(), 10);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].Int(), 11);

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Conversion VR UT
 */
BOOST_AUTO_TEST_CASE(ConversionUT)
{
    DcmDataset* dataset = new DcmDataset();
    // Be carefull: putAndInsertOFStringArray for UT add only 1 value !!!
    dataset->putAndInsertOFStringArray(DCM_PixelDataProviderURL, "value01\\value02");

    dopamine::converterBSON::DataSetToBSON datasettobson;
    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    mongo::BSONObj::iterator it = query_dataset.begin();

    mongo::BSONElement element = it.next();
    mongo::BSONObj object = element.Obj();
    BOOST_CHECK_EQUAL(element.fieldName(), "00287fe0");
    BOOST_CHECK_EQUAL(object.getField("vr").String(), "UT");
    BOOST_CHECK_EQUAL(object.getField("Value").Array().size(), 1);
    BOOST_CHECK_EQUAL(object.getField("Value").Array()[0].String(), "value01\\value02");
    //BOOST_CHECK_EQUAL(object.getField("Value").Array()[1].String(), "value02");

    BOOST_CHECK_EQUAL(it.more(), false);

    delete dataset;
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Filter
 */
BOOST_AUTO_TEST_CASE(Filter_Exclude_all)
{
    dopamine::converterBSON::DataSetToBSON datasettobson;

    // set_default_filter
    datasettobson.set_default_filter(dopamine::converterBSON::DataSetToBSON::FilterAction::EXCLUDE);

    DcmDataset* dataset = new DcmDataset();
    dataset->putAndInsertOFStringArray(DCM_Modality, "value01\\value02");
    dataset->putAndInsertOFStringArray(DCM_PatientBirthDate, "value01\\value02");

    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    BOOST_CHECK_EQUAL(query_dataset.isEmpty(), true);
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Filter
 */
BOOST_AUTO_TEST_CASE(Filter_Exclude_field)
{
    dopamine::converterBSON::DataSetToBSON datasettobson;

    // set_default_filter
    datasettobson.set_default_filter(dopamine::converterBSON::DataSetToBSON::FilterAction::INCLUDE);
    std::vector<dopamine::converterBSON::DataSetToBSON::Filter> filters;
    filters.push_back(std::make_pair(dopamine::converterBSON::TagMatch::New(DCM_PatientBirthDate),
                      dopamine::converterBSON::DataSetToBSON::FilterAction::EXCLUDE));
    datasettobson.set_filters(filters);

    DcmDataset* dataset = new DcmDataset();
    dataset->putAndInsertOFStringArray(DCM_Modality, "value01\\value02");
    dataset->putAndInsertOFStringArray(DCM_PatientBirthDate, "value01\\value02");

    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    BOOST_CHECK_EQUAL(query_dataset.hasField("00080060"), true);
    BOOST_CHECK_EQUAL(query_dataset.hasField("00100030"), false);
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Set Specific character set with Dataset element
 */
BOOST_AUTO_TEST_CASE(Set_Specific_Character_Set)
{
    dopamine::converterBSON::DataSetToBSON datasettobson;

    // set_default_filter
    datasettobson.set_default_filter(dopamine::converterBSON::DataSetToBSON::FilterAction::INCLUDE);

    DcmDataset* dataset = new DcmDataset();
    dataset->putAndInsertOFStringArray(DCM_SpecificCharacterSet, "ISO_IR 192");
    dataset->putAndInsertOFStringArray(DCM_Modality, "value01\\value02");

    BOOST_CHECK_EQUAL(datasettobson.get_specific_character_set(), "");

    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    BOOST_CHECK_EQUAL(query_dataset.hasField("00080060"), true);
    BOOST_CHECK_EQUAL(datasettobson.get_specific_character_set(), "ISO_IR 192");
}

/*************************** TEST Nominal *******************************/
/**
 * Nominal test case: Ignore Group length attribute
 */
BOOST_AUTO_TEST_CASE(Group_length_tag)
{
    dopamine::converterBSON::DataSetToBSON datasettobson;

    // set_default_filter
    datasettobson.set_default_filter(dopamine::converterBSON::DataSetToBSON::FilterAction::INCLUDE);

    DcmDataset* dataset = new DcmDataset();
    dataset->putAndInsertOFStringArray(DCM_Modality, "value01\\value02");
    DcmElement * element = NULL;
    dataset->insertEmptyElement(DCM_FileMetaInformationGroupLength);
    dataset->findAndGetElement(DCM_FileMetaInformationGroupLength, element);
    std::vector<Uint32> vectoruint32 = {10};
    element->putUint32Array(&vectoruint32[0], 1);

    mongo::BSONObj const query_dataset = datasettobson.from_dataset(dataset);

    BOOST_CHECK_EQUAL(query_dataset.hasField("00080060"), true);
    BOOST_CHECK_EQUAL(query_dataset.hasField("00020000"), false);
}

/*************************** TEST Error *********************************/
/**
 * Error test case: set_specific_character_set => bad value
 */
BOOST_AUTO_TEST_CASE(Specific_charset_badvalue)
{
    dopamine::converterBSON::DataSetToBSON datasettobson;

    // set_specific_character_set
    BOOST_REQUIRE_THROW(datasettobson.set_specific_character_set("badvalue"),
                        std::runtime_error);
}

/*************************** TEST Error *********************************/
/**
 * Error test case: set_specific_character_set => multi-valued
 */
BOOST_AUTO_TEST_CASE(Specific_charset_multiValue)
{
    dopamine::converterBSON::DataSetToBSON datasettobson;

    // set_specific_character_set
    BOOST_REQUIRE_THROW(datasettobson.set_specific_character_set("ISO_IR 192\\GB18030"),
                        std::runtime_error);
}

/*************************** TEST Error *********************************/
/**
 * Error test case: Throw Unhandled VR
 */
BOOST_AUTO_TEST_CASE(Unhandled_VR)
{
    DcmDataset* dataset = new DcmDataset();
    DcmAttributeTag* element = new DcmAttributeTag(DcmTag(0x9998,0x9998,DcmVR(EVR_na)));
    std::string test = "(0008,0020)\(0008,0030)";
    OFCondition cond = element->putString(test.c_str());
    dataset->insert(element);

    dopamine::converterBSON::DataSetToBSON datasettobson;
    BOOST_REQUIRE_THROW(datasettobson.from_dataset(dataset);,
                        std::runtime_error);

    delete dataset;
}