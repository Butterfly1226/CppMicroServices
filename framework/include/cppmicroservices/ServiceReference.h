/*=============================================================================

  Library: CppMicroServices

  Copyright (c) The CppMicroServices developers. See the COPYRIGHT
  file at the top-level directory of this distribution and at
  https://github.com/CppMicroServices/CppMicroServices/COPYRIGHT .

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/

#ifndef CPPMICROSERVICES_SERVICEREFERENCE_H
#define CPPMICROSERVICES_SERVICEREFERENCE_H

#include "cppmicroservices/ServiceInterface.h"
#include "cppmicroservices/ServiceReferenceBase.h"

namespace cppmicroservices
{

    /**
    \defgroup gr_servicereference ServiceReference

    \brief Groups ServiceReference related symbols.
    */

    /**
     * \ingroup MicroServices
     * \ingroup gr_servicereference
     *
     * A reference to a service.
     *
     * <p>
     * The framework returns <code>ServiceReference</code> objects from the
     * <code>BundleContext::GetServiceReference</code> and
     * <code>BundleContext::GetServiceReferences</code> methods.
     * <p>
     * A <code>ServiceReference</code> object may be shared between bundles and
     * can be used to examine the properties of the service and to get the service
     * object.
     * <p>
     * Every service registered in the framework has a unique
     * <code>ServiceRegistration</code> object and may have multiple, distinct
     * <code>ServiceReference</code> objects referring to it.
     * <code>ServiceReference</code> objects associated with a
     * <code>ServiceRegistration</code> are considered equal
     * (more specifically, their <code>operator==()</code>
     * method will return <code>true</code> when compared).
     * <p>
     * If the same service object is registered multiple times,
     * <code>ServiceReference</code> objects associated with different
     * <code>ServiceRegistration</code> objects are not equal.
     *
     * @tparam S The class type of the service interface
     * @see BundleContext::GetServiceReference
     * @see BundleContext::GetServiceReferences
     * @see BundleContext::GetService
     */
    template <class S>
    class ServiceReference : public ServiceReferenceBase
    {

      public:
        using ServiceType = S;

        /**
         * Creates an invalid ServiceReference object. You can use
         * this object in boolean expressions and it will evaluate to
         * <code>false</code>.
         */
        ServiceReference() : ServiceReferenceBase() {}

        ServiceReference(ServiceReference const&) = default;
        ServiceReference& operator=(ServiceReference const&) = default;

        ServiceReference(ServiceReferenceBase const& base) : ServiceReferenceBase(base)
        {
            std::string const interfaceId(us_service_interface_iid<S>());
            if (GetInterfaceId() != interfaceId)
            {
                if (this->IsConvertibleTo(interfaceId))
                {
                    this->SetInterfaceId(interfaceId);
                }
                else
                {
                    this->operator=(nullptr);
                }
            }
        }

        using ServiceReferenceBase::operator=;

        using ServiceReferenceBase::operator==;

        using ServiceReferenceBase::Hash;
    };

    /**
     * \cond
     *
     * Specialization for void, representing a generic service
     * reference not bound to any interface identifier.
     *
     */
    template <>
    class ServiceReference<void> : public ServiceReferenceBase
    {

      public:
        /**
         * Creates an invalid ServiceReference object. You can use
         * this object in boolean expressions and it will evaluate to
         * <code>false</code>.
         */
        ServiceReference() : ServiceReferenceBase() {}

        ServiceReference(ServiceReference const&) = default;
        ServiceReference& operator=(ServiceReference const&) = default;

        ServiceReference(ServiceReferenceBase const& base) : ServiceReferenceBase(base) {}

        using ServiceReferenceBase::operator=;

        using ServiceReferenceBase::operator==;

        using ServiceReferenceBase::Hash;

        using ServiceType = void;
    };
    /// \endcond

    /**
     * \ingroup MicroServices
     * \ingroup gr_servicereference
     *
     * A service reference of unknown type, which is not bound to any
     * interface identifier.
     */
    using ServiceReferenceU = ServiceReference<void>;

    /**
     * \ingroup MicroServices
     * \ingroup gr_servicereference
     *
     * A method to retrieve a <code>ServiceObject</code>'s original <code>ServiceReference<void></code>
     *
     */
    US_Framework_EXPORT ServiceReferenceU ServiceReferenceFromService(std::shared_ptr<void> const& s);

    /**
     * \ingroup MicroServices
     * \ingroup gr_servicereference
     *
     * A method to retrieve a <code>ServiceObject<T></code>'s original <code>ServiceReference<U></code>
     *
     * @tparam T The class type of the <code>ServiceObject</code>
     * @tparam U The class type of the <code>ServiceReference</code>. Defaults to <code>T</code> if not specified.
     */
    template <typename T, typename U = T>
    ServiceReference<U>
    ServiceReferenceFromService(std::shared_ptr<T> const& s)
    {
        return ServiceReference<U>(ServiceReferenceFromService(std::static_pointer_cast<void>(s)));
    }
} // namespace cppmicroservices

namespace std
{
    template <class S>
    struct hash<cppmicroservices::ServiceReference<S>>
    {
        std::size_t
        operator()(cppmicroservices::ServiceReference<S> const& arg) const
        {
            return arg.Hash();
        }
    };
} // namespace std

#endif // CPPMICROSERVICES_SERVICEREFERENCE_H
