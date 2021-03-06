/*************************************************************************
 * dopamine - Copyright (C) Universite de Strasbourg
 * Distributed under the terms of the CeCILL-B license, as published by
 * the CEA-CNRS-INRIA. Refer to the LICENSE file or to
 * http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
 * for details.
 ************************************************************************/

#ifndef _9533ce45_f1ca_4bea_ba12_3d77495bacd6
#define _9533ce45_f1ca_4bea_ba12_3d77495bacd6

#include <string>
#include <vector>

#include <mongo/bson/bson.h>
#include <mongo/client/dbclient.h>

#include <odil/DataSet.h>

#include "dopamine/AccessControlList.h"
#include "dopamine/archive/Storage.h"

namespace dopamine
{

namespace archive
{

/// @brief Helper for the data sets generators (Find, Get and Move).
class DataSetGeneratorHelper
{
public:
    /// @brief Constructor.
    DataSetGeneratorHelper(
        mongo::DBClientConnection & connection, AccessControlList const & acl,
        std::string const & database, std::string const & bulk_database,
        std::string const & principal, std::string const & service);

    /**
     * @brief Check that the principal is allowed to use the service, throw
     * an exception otherwise.
     */
    void check_acl() const;

    /**
     * @brief Get the condition and projection associated with the data sets and
     * the ACL constraints.
     */
    void get_condition_and_projection(
        odil::DataSet const & data_set,
        mongo::BSONObjBuilder & condition_builder,
        mongo::BSONObjBuilder & projection_builder) const;

    /// @brief Set and initialize the results iterator.
    void set_results(std::vector<mongo::BSONObj> const & results);

    /// @brief Test whether all elements have been generated.
    bool done() const;

    /// @brief Prepare the next element.
    void next();

    /// @brief Return the current element.
    mongo::BSONObj const & get() const;

    /// @brief Return the number of responses.
    unsigned int count() const;

    /**
     * @brief Return the data set with given SOP instance UID; throw an
     * exception if no such data set is stored.
     */
    odil::DataSet retrieve(std::string const & sop_instance_uid) const;

private:
    mongo::DBClientConnection & _connection;
    AccessControlList const & _acl;
    Storage _storage;

    std::string _principal;
    std::string _service;

    std::vector<mongo::BSONObj> _results;
    std::vector<mongo::BSONObj>::const_iterator _results_iterator;
};

} // namespace archive

} // namespace dopamine


#endif // _9533ce45_f1ca_4bea_ba12_3d77495bacd6
