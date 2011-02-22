/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.17 $ of : mfd-data-access.m2c,v $ 
 *
 * $Id$
 */
/*
 * standard Net-SNMP includes 
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * include our parent header 
 */
#include "inetCidrRouteTable.h"


#include "inetCidrRouteTable_data_access.h"

/** @ingroup interface 
 * @addtogroup data_access data_access: Routines to access data
 *
 * These routines are used to locate the data used to satisfy
 * requests.
 * 
 * @{
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table inetCidrRouteTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * IP-FORWARD-MIB::inetCidrRouteTable is subid 7 of ipForward.
 * Its status is Current.
 * OID: .1.3.6.1.2.1.4.24.7, length: 9
 */

/**
 * initialization for inetCidrRouteTable data access
 *
 * This function is called during startup to allow you to
 * allocate any resources you need for the data table.
 *
 * @param inetCidrRouteTable_reg
 *        Pointer to inetCidrRouteTable_registration
 *
 * @retval MFD_SUCCESS : success.
 * @retval MFD_ERROR   : unrecoverable error.
 */
int
inetCidrRouteTable_init_data(inetCidrRouteTable_registration *
                             inetCidrRouteTable_reg)
{
    DEBUGMSGTL(("verbose:inetCidrRouteTable:inetCidrRouteTable_init_data",
                "called\n"));

    /*
     * TODO:303:o: Initialize inetCidrRouteTable data.
     */

    return MFD_SUCCESS;
}                               /* inetCidrRouteTable_init_data */

/**
 * container overview
 *
 */

/**
 * container initialization
 *
 * @param container_ptr_ptr A pointer to a container pointer. If you
 *        create a custom container, use this parameter to return it
 *        to the MFD helper. If set to NULL, the MFD helper will
 *        allocate a container for you.
 * @param  cache A pointer to a cache structure. You can set the timeout
 *         and other cache flags using this pointer.
 *
 *  This function is called at startup to allow you to customize certain
 *  aspects of the access method. For the most part, it is for advanced
 *  users. The default code should suffice for most cases. If no custom
 *  container is allocated, the MFD code will create one for your.
 *
 *  This is also the place to set up cache behavior. The default, to
 *  simply set the cache timeout, will work well with the default
 *  container. If you are using a custom container, you may want to
 *  look at the cache helper documentation to see if there are any
 *  flags you want to set.
 *
 * @remark
 *  This would also be a good place to do any initialization needed
 *  for you data source. For example, opening a connection to another
 *  process that will supply the data, opening a database, etc.
 */
void
inetCidrRouteTable_container_init(netsnmp_container **container_ptr_ptr,
                                  netsnmp_cache * cache)
{
    DEBUGMSGTL(("verbose:inetCidrRouteTable:inetCidrRouteTable_container_init", "called\n"));

    if (NULL == container_ptr_ptr) {
        snmp_log(LOG_ERR,
                 "bad container param to inetCidrRouteTable_container_init\n");
        return;
    }

    /*
     * For advanced users, you can use a custom container. If you
     * do not create one, one will be created for you.
     */
    *container_ptr_ptr = NULL;

    if (NULL == cache) {
        snmp_log(LOG_ERR,
                 "bad cache param to inetCidrRouteTable_container_init\n");
        return;
    }

    /*
     * TODO:345:A: Set up inetCidrRouteTable cache properties.
     *
     * Also for advanced users, you can set parameters for the
     * cache. Do not change the magic pointer, as it is used
     * by the MFD helper. To completely disable caching, set
     * cache->enabled to 0.
     */
    cache->timeout = INETCIDRROUTETABLE_CACHE_TIMEOUT;  /* seconds */
}                               /* inetCidrRouteTable_container_init */

/**
 * check entry for update
 */
static void
_snarf_route_entry(netsnmp_route_entry *route_entry,
                   netsnmp_container *container)
{
    inetCidrRouteTable_rowreq_ctx *rowreq_ctx;

    netsnmp_assert(NULL != route_entry);
    netsnmp_assert(NULL != container);

    /*
     * per  inetCidrRouteType:
     *
     * Routes which do not result in traffic forwarding or 
     * rejection should not be displayed even if the  
     * implementation keeps them stored internally.
     */
    if (route_entry->rt_type == 0) {    /* set when route not up */
        DEBUGMSGT(("verbose:inetCidrRouteTable:inetCidrRouteTable_cache_load", "skipping route\n"));
        netsnmp_access_route_entry_free(route_entry);
        return;
    }

    /*
     * allocate an row context and set the index(es), then add it to
     * the container
     */
    rowreq_ctx = inetCidrRouteTable_allocate_rowreq_ctx(route_entry, NULL);
    if ((NULL != rowreq_ctx) &&
        (MFD_SUCCESS == inetCidrRouteTable_indexes_set
         (rowreq_ctx, route_entry->rt_dest_type,
          route_entry->rt_dest, route_entry->rt_dest_len,
          route_entry->rt_pfx_len,
          route_entry->rt_policy, route_entry->rt_policy_len,
          route_entry->rt_nexthop_type,
          route_entry->rt_nexthop, route_entry->rt_nexthop_len))) {
        CONTAINER_INSERT(container, rowreq_ctx);
        rowreq_ctx->row_status = ROWSTATUS_ACTIVE;
    } else {
        if (rowreq_ctx) {
            snmp_log(LOG_ERR, "error setting index while loading "
                     "inetCidrRoute cache.\n");
            inetCidrRouteTable_release_rowreq_ctx(rowreq_ctx);
        } else
            netsnmp_access_route_entry_free(route_entry);
    }
}

/**
 * container shutdown
 *
 * @param container_ptr A pointer to the container.
 *
 *  This function is called at shutdown to allow you to customize certain
 *  aspects of the access method. For the most part, it is for advanced
 *  users. The default code should suffice for most cases.
 *
 *  This function is called before inetCidrRouteTable_container_free().
 *
 * @remark
 *  This would also be a good place to do any cleanup needed
 *  for you data source. For example, closing a connection to another
 *  process that supplied the data, closing a database, etc.
 */
void
inetCidrRouteTable_container_shutdown(netsnmp_container *container_ptr)
{
    DEBUGMSGTL(("verbose:inetCidrRouteTable:inetCidrRouteTable_container_shutdown", "called\n"));

    if (NULL == container_ptr) {
        snmp_log(LOG_ERR,
                 "bad params to inetCidrRouteTable_container_shutdown\n");
        return;
    }

}                               /* inetCidrRouteTable_container_shutdown */

/**
 * load initial data
 *
 * TODO:350:M: Implement inetCidrRouteTable data load
 * This function will also be called by the cache helper to load
 * the container again (after the container free function has been
 * called to free the previous contents).
 *
 * @param container container to which items should be inserted
 *
 * @retval MFD_SUCCESS              : success.
 * @retval MFD_RESOURCE_UNAVAILABLE : Can't access data source
 * @retval MFD_ERROR                : other error.
 *
 *  This function is called to load the index(es) (and data, optionally)
 *  for the every row in the data set.
 *
 * @remark
 *  While loading the data, the only important thing is the indexes.
 *  If access to your data is cheap/fast (e.g. you have a pointer to a
 *  structure in memory), it would make sense to update the data here.
 *  If, however, the accessing the data invovles more work (e.g. parsing
 *  some other existing data, or peforming calculations to derive the data),
 *  then you can limit yourself to setting the indexes and saving any
 *  information you will need later. Then use the saved information in
 *  inetCidrRouteTable_row_prep() for populating data.
 *
 * @note
 *  If you need consistency between rows (like you want statistics
 *  for each row to be from the same time frame), you should set all
 *  data here.
 *
 */
int
inetCidrRouteTable_container_load(netsnmp_container *container)
{
    netsnmp_container *route_container;

    DEBUGMSGTL(("verbose:inetCidrRouteTable:inetCidrRouteTable_container_load", "called\n"));

    /*
     * TODO:351:M: |-> Load/update data in the inetCidrRouteTable container.
     * loop over your inetCidrRouteTable data, allocate a rowreq context,
     * set the index(es) [and data, optionally] and insert into
     * the container.
     *
     * we use the netsnmp data access api to get the data
     */
    route_container =
        netsnmp_access_route_container_load(NULL,
                                            NETSNMP_ACCESS_ROUTE_LOAD_NOFLAGS);
    DEBUGMSGT(("verbose:inetCidrRouteTable:inetCidrRouteTable_cache_load",
               "%d records\n", (int)CONTAINER_SIZE(route_container)));

    if (NULL == route_container)
        return MFD_RESOURCE_UNAVAILABLE;        /* msg already logged */

    /*
     * we just got a fresh copy of route data. snarf data
     */
    CONTAINER_FOR_EACH(route_container,
                       (netsnmp_container_obj_func *) _snarf_route_entry,
                       container);

    /*
     * free the container. we've either claimed each ifentry, or released it,
     * so the dal function doesn't need to clear the container.
     */
    netsnmp_access_route_container_free(route_container,
                                        NETSNMP_ACCESS_ROUTE_FREE_DONT_CLEAR);

    DEBUGMSGT(("verbose:inetCidrRouteTable:inetCidrRouteTable_cache_load",
               "%d records\n", (int)CONTAINER_SIZE(container)));

    return MFD_SUCCESS;
}                               /* inetCidrRouteTable_container_load */

/**
 * container clean up
 *
 * @param container container with all current items
 *
 *  This optional callback is called prior to all
 *  item's being removed from the container. If you
 *  need to do any processing before that, do it here.
 *
 * @note
 *  The MFD helper will take care of releasing all the row contexts.
 *
 */
void
inetCidrRouteTable_container_free(netsnmp_container *container)
{
    DEBUGMSGTL(("verbose:inetCidrRouteTable:inetCidrRouteTable_container_free", "called\n"));

    /*
     * TODO:380:M: Free inetCidrRouteTable container data.
     */
}                               /* inetCidrRouteTable_container_free */

/**
 * prepare row for processing.
 *
 *  When the agent has located the row for a request, this function is
 *  called to prepare the row for processing. If you fully populated
 *  the data context during the index setup phase, you may not need to
 *  do anything.
 *
 * @param rowreq_ctx pointer to a context.
 *
 * @retval MFD_SUCCESS     : success.
 * @retval MFD_ERROR       : other error.
 */
int
inetCidrRouteTable_row_prep(inetCidrRouteTable_rowreq_ctx * rowreq_ctx)
{
    DEBUGMSGTL(("verbose:inetCidrRouteTable:inetCidrRouteTable_row_prep",
                "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:390:o: Prepare row for request.
     * If populating row data was delayed, this is the place to
     * fill in the row for this request.
     */
    if (MFD_ROW_CREATED & rowreq_ctx->rowreq_flags) {
        memcpy(rowreq_ctx->data->rt_dest,
               rowreq_ctx->tbl_idx.inetCidrRouteDest,
               rowreq_ctx->tbl_idx.inetCidrRouteDest_len);
        rowreq_ctx->data->rt_dest_len =
            rowreq_ctx->tbl_idx.inetCidrRouteDest_len;
        rowreq_ctx->data->rt_dest_type =
            rowreq_ctx->tbl_idx.inetCidrRouteDestType;

        memcpy(rowreq_ctx->data->rt_nexthop,
               rowreq_ctx->tbl_idx.inetCidrRouteNextHop,
               rowreq_ctx->tbl_idx.inetCidrRouteNextHop_len);
        rowreq_ctx->data->rt_nexthop_len =
            rowreq_ctx->tbl_idx.inetCidrRouteNextHop_len;
        rowreq_ctx->data->rt_nexthop_type =
            rowreq_ctx->tbl_idx.inetCidrRouteNextHopType;

        rowreq_ctx->data->rt_pfx_len =
            rowreq_ctx->tbl_idx.inetCidrRoutePfxLen;

        rowreq_ctx->data->rt_policy_len =
            rowreq_ctx->tbl_idx.inetCidrRoutePolicy_len;
        rowreq_ctx->data->rt_policy =
            rowreq_ctx->tbl_idx.inetCidrRoutePolicy;
        /*
         * policy pointing to rowreq array, set flag so
         * it won't be freed
         */
        rowreq_ctx->data->flags |= NETSNMP_ACCESS_ROUTE_POLICY_STATIC;
    }


    return MFD_SUCCESS;
}                               /* inetCidrRouteTable_row_prep */

/*
 * TODO:420:r: Implement inetCidrRouteTable index validation.
 */
/*---------------------------------------------------------------------
 * IP-FORWARD-MIB::inetCidrRouteEntry.inetCidrRouteDestType
 * inetCidrRouteDestType is subid 1 of inetCidrRouteEntry.
 * Its status is Current, and its access level is NoAccess.
 * OID: .1.3.6.1.2.1.4.24.7.1.1
 * Description:
The type of the inetCidrRouteDest address, as defined  
                in the InetAddress MIB. 

                Only those address types that may appear in an actual 
                routing table are allowed as values of this object.
 *
 * Attributes:
 *   accessible 0     isscalar 0     enums  1      hasdefval 0
 *   readable   0     iscolumn 1     ranges 0      hashint   0
 *   settable   0
 *
 * Enum range: 5/8. Values:  unknown(0), ipv4(1), ipv6(2), ipv4z(3), ipv6z(4), dns(16)
 *
 * Its syntax is InetAddressType (based on perltype INTEGER)
 * The net-snmp type is ASN_INTEGER. The C type decl is long (u_long)
 *
 *
 *
 * NOTE: NODE inetCidrRouteDestType IS NOT ACCESSIBLE
 *
 *
 */
/**
 * check validity of inetCidrRouteDestType index portion
 *
 * @retval MFD_SUCCESS   : the incoming value is legal
 * @retval MFD_ERROR     : the incoming value is NOT legal
 *
 * @note this is not the place to do any checks for the sanity
 *       of multiple indexes. Those types of checks should be done in the
 *       inetCidrRouteTable_validate_index() function.
 *
 * @note Also keep in mind that if the index refers to a row in this or
 *       some other table, you can't check for that row here to make
 *       decisions, since that row might not be created yet, but may
 *       be created during the processing this request. If you have
 *       such checks, they should be done in the check_dependencies
 *       function, because any new/deleted/changed rows should be
 *       available then.
 *
 * The following checks have already been done for you:
 *    The value is one of  unknown(0), ipv4(1), ipv6(2), ipv4z(3), ipv6z(4), dns(16)
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 */
int
inetCidrRouteDestType_check_index(inetCidrRouteTable_rowreq_ctx *
                                  rowreq_ctx)
{
    DEBUGMSGTL(("verbose:inetCidrRouteTable:inetCidrRouteDestType_check_index", "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:426:M: |-> Check inetCidrRouteTable index inetCidrRouteDestType.
     * check that index value in the table context is legal.
     * (rowreq_ctx->tbl_index.inetCidrRouteDestType)
     */
    /*
     * we don't support all types
     */
    switch (rowreq_ctx->tbl_idx.inetCidrRouteDestType) {

    case INETADDRESSTYPE_IPV4:
    case INETADDRESSTYPE_IPV6:
        break;

    case INETADDRESSTYPE_UNKNOWN:
    case INETADDRESSTYPE_IPV4Z:
    case INETADDRESSTYPE_IPV6Z:
    case INETADDRESSTYPE_DNS:
        /*
         * fall through 
         */

    default:
        return MFD_ERROR;
    }

    return MFD_SUCCESS;         /* inetCidrRouteDestType index ok */
}                               /* inetCidrRouteDestType_check_index */

/*---------------------------------------------------------------------
 * IP-FORWARD-MIB::inetCidrRouteEntry.inetCidrRouteDest
 * inetCidrRouteDest is subid 2 of inetCidrRouteEntry.
 * Its status is Current, and its access level is NoAccess.
 * OID: .1.3.6.1.2.1.4.24.7.1.2
 * Description:
The destination IP address of this route. 

                The type of this address is determined by the value of 
                the inetCidrRouteDestType object. 

                The values for the index objects inetCidrRouteDest and 
                inetCidrRoutePfxLen must be consistent.  When the value 
                of inetCidrRouteDest (excluding the zone index, if one 
                is present) is x, then the bitwise logical-AND 
                of x with the value of the mask formed from the 
                corresponding index object inetCidrRoutePfxLen MUST be 
                equal to x.  If not, then the index pair is not 
                consistent and an inconsistentName error must be 
                returned on SET or CREATE requests.
 *
 * Attributes:
 *   accessible 0     isscalar 0     enums  0      hasdefval 0
 *   readable   0     iscolumn 1     ranges 1      hashint   0
 *   settable   0
 *
 * Ranges:  0 - 255;
 *
 * Its syntax is InetAddress (based on perltype OCTETSTR)
 * The net-snmp type is ASN_OCTET_STR. The C type decl is char (char)
 * This data type requires a length.  (Max 255)
 *
 *
 *
 * NOTE: NODE inetCidrRouteDest IS NOT ACCESSIBLE
 *
 *
 */
/**
 * check validity of inetCidrRouteDest index portion
 *
 * @retval MFD_SUCCESS   : the incoming value is legal
 * @retval MFD_ERROR     : the incoming value is NOT legal
 *
 * @note this is not the place to do any checks for the sanity
 *       of multiple indexes. Those types of checks should be done in the
 *       inetCidrRouteTable_validate_index() function.
 *
 * @note Also keep in mind that if the index refers to a row in this or
 *       some other table, you can't check for that row here to make
 *       decisions, since that row might not be created yet, but may
 *       be created during the processing this request. If you have
 *       such checks, they should be done in the check_dependencies
 *       function, because any new/deleted/changed rows should be
 *       available then.
 *
 * The following checks have already been done for you:
 *    The length is in (one of) the range set(s):  0 - 255
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 */
int
inetCidrRouteDest_check_index(inetCidrRouteTable_rowreq_ctx * rowreq_ctx)
{
    DEBUGMSGTL(("verbose:inetCidrRouteTable:inetCidrRouteDest_check_index",
                "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:426:M: |-> Check inetCidrRouteTable index inetCidrRouteDest.
     * check that index value in the table context is legal.
     * (rowreq_ctx->tbl_index.inetCidrRouteDest)
     */

    return MFD_SUCCESS;         /* inetCidrRouteDest index ok */
}                               /* inetCidrRouteDest_check_index */

/*---------------------------------------------------------------------
 * IP-FORWARD-MIB::inetCidrRouteEntry.inetCidrRoutePfxLen
 * inetCidrRoutePfxLen is subid 3 of inetCidrRouteEntry.
 * Its status is Current, and its access level is NoAccess.
 * OID: .1.3.6.1.2.1.4.24.7.1.3
 * Description:
Indicates the number of leading one bits which form the  
                mask to be logical-ANDed with the destination address  
                before being compared to the value in the 
                inetCidrRouteDest field. 

                The values for the index objects inetCidrRouteDest and 
                inetCidrRoutePfxLen must be consistent.  When the value 
                of inetCidrRouteDest (excluding the zone index, if one 
                is present) is x, then the bitwise logical-AND 
                of x with the value of the mask formed from the 
                corresponding index object inetCidrRoutePfxLen MUST be 
                equal to x.  If not, then the index pair is not 
                consistent and an inconsistentName error must be 
                returned on SET or CREATE requests.
 *
 * Attributes:
 *   accessible 0     isscalar 0     enums  0      hasdefval 0
 *   readable   0     iscolumn 1     ranges 1      hashint   1
 *   settable   0
 *   hint: d
 *
 * Ranges:  0 - 2040;
 *
 * Its syntax is InetAddressPrefixLength (based on perltype UNSIGNED32)
 * The net-snmp type is ASN_UNSIGNED. The C type decl is u_long (u_long)
 *
 *
 *
 * NOTE: NODE inetCidrRoutePfxLen IS NOT ACCESSIBLE
 *
 *
 */
/**
 * check validity of inetCidrRoutePfxLen index portion
 *
 * @retval MFD_SUCCESS   : the incoming value is legal
 * @retval MFD_ERROR     : the incoming value is NOT legal
 *
 * @note this is not the place to do any checks for the sanity
 *       of multiple indexes. Those types of checks should be done in the
 *       inetCidrRouteTable_validate_index() function.
 *
 * @note Also keep in mind that if the index refers to a row in this or
 *       some other table, you can't check for that row here to make
 *       decisions, since that row might not be created yet, but may
 *       be created during the processing this request. If you have
 *       such checks, they should be done in the check_dependencies
 *       function, because any new/deleted/changed rows should be
 *       available then.
 *
 * The following checks have already been done for you:
 *    The value is in (one of) the range set(s):  0 - 2040
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 */
int
inetCidrRoutePfxLen_check_index(inetCidrRouteTable_rowreq_ctx * rowreq_ctx)
{
    DEBUGMSGTL(("verbose:inetCidrRouteTable:inetCidrRoutePfxLen_check_index", "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:426:M: |-> Check inetCidrRouteTable index inetCidrRoutePfxLen.
     * check that index value in the table context is legal.
     * (rowreq_ctx->tbl_index.inetCidrRoutePfxLen)
     */

    return MFD_SUCCESS;         /* inetCidrRoutePfxLen index ok */
}                               /* inetCidrRoutePfxLen_check_index */

/*---------------------------------------------------------------------
 * IP-FORWARD-MIB::inetCidrRouteEntry.inetCidrRoutePolicy
 * inetCidrRoutePolicy is subid 4 of inetCidrRouteEntry.
 * Its status is Current, and its access level is NoAccess.
 * OID: .1.3.6.1.2.1.4.24.7.1.4
 * Description:
This object is an opaque object without any defined 
                semantics.  Its purpose is to serve as an additional 
                index which may delineate between multiple entries to 
                the same destination.  The value { 0 0 } shall be used 
                as the default value for this object.
 *
 * Attributes:
 *   accessible 0     isscalar 0     enums  0      hasdefval 0
 *   readable   0     iscolumn 1     ranges 0      hashint   0
 *   settable   0
 *
 *
 * Its syntax is OBJECTID (based on perltype OBJECTID)
 * The net-snmp type is ASN_OBJECT_ID. The C type decl is oid (oid)
 * This data type requires a length.  (Max 2040)
 *
 *
 *
 * NOTE: NODE inetCidrRoutePolicy IS NOT ACCESSIBLE
 *
 *
 */
/**
 * check validity of inetCidrRoutePolicy index portion
 *
 * @retval MFD_SUCCESS   : the incoming value is legal
 * @retval MFD_ERROR     : the incoming value is NOT legal
 *
 * @note this is not the place to do any checks for the sanity
 *       of multiple indexes. Those types of checks should be done in the
 *       inetCidrRouteTable_validate_index() function.
 *
 * @note Also keep in mind that if the index refers to a row in this or
 *       some other table, you can't check for that row here to make
 *       decisions, since that row might not be created yet, but may
 *       be created during the processing this request. If you have
 *       such checks, they should be done in the check_dependencies
 *       function, because any new/deleted/changed rows should be
 *       available then.
 *
 * The following checks have already been done for you:
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 */
int
inetCidrRoutePolicy_check_index(inetCidrRouteTable_rowreq_ctx * rowreq_ctx)
{
    DEBUGMSGTL(("verbose:inetCidrRouteTable:inetCidrRoutePolicy_check_index", "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:426:M: |-> Check inetCidrRouteTable index inetCidrRoutePolicy.
     * check that index value in the table context is legal.
     * (rowreq_ctx->tbl_index.inetCidrRoutePolicy)
     */

    return MFD_SUCCESS;         /* inetCidrRoutePolicy index ok */
}                               /* inetCidrRoutePolicy_check_index */

/*---------------------------------------------------------------------
 * IP-FORWARD-MIB::inetCidrRouteEntry.inetCidrRouteNextHopType
 * inetCidrRouteNextHopType is subid 5 of inetCidrRouteEntry.
 * Its status is Current, and its access level is NoAccess.
 * OID: .1.3.6.1.2.1.4.24.7.1.5
 * Description:
The type of the inetCidrRouteNextHop address, as  
                defined in the InetAddress MIB. 

                Value should be set to unknown(0) for non-remote  
                routes. 

                Only those address types that may appear in an actual 
                routing table are allowed as values of this object.
 *
 * Attributes:
 *   accessible 0     isscalar 0     enums  1      hasdefval 0
 *   readable   0     iscolumn 1     ranges 0      hashint   0
 *   settable   0
 *
 * Enum range: 5/8. Values:  unknown(0), ipv4(1), ipv6(2), ipv4z(3), ipv6z(4), dns(16)
 *
 * Its syntax is InetAddressType (based on perltype INTEGER)
 * The net-snmp type is ASN_INTEGER. The C type decl is long (u_long)
 *
 *
 *
 * NOTE: NODE inetCidrRouteNextHopType IS NOT ACCESSIBLE
 *
 *
 */
/**
 * check validity of inetCidrRouteNextHopType index portion
 *
 * @retval MFD_SUCCESS   : the incoming value is legal
 * @retval MFD_ERROR     : the incoming value is NOT legal
 *
 * @note this is not the place to do any checks for the sanity
 *       of multiple indexes. Those types of checks should be done in the
 *       inetCidrRouteTable_validate_index() function.
 *
 * @note Also keep in mind that if the index refers to a row in this or
 *       some other table, you can't check for that row here to make
 *       decisions, since that row might not be created yet, but may
 *       be created during the processing this request. If you have
 *       such checks, they should be done in the check_dependencies
 *       function, because any new/deleted/changed rows should be
 *       available then.
 *
 * The following checks have already been done for you:
 *    The value is one of  unknown(0), ipv4(1), ipv6(2), ipv4z(3), ipv6z(4), dns(16)
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 */
int
inetCidrRouteNextHopType_check_index(inetCidrRouteTable_rowreq_ctx *
                                     rowreq_ctx)
{
    DEBUGMSGTL(("verbose:inetCidrRouteTable:inetCidrRouteNextHopType_check_index", "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:426:M: |-> Check inetCidrRouteTable index inetCidrRouteNextHopType.
     * check that index value in the table context is legal.
     * (rowreq_ctx->tbl_index.inetCidrRouteNextHopType)
     */
    /*
     * we don't support all types
     */
    switch (rowreq_ctx->tbl_idx.inetCidrRouteDestType) {

    case INETADDRESSTYPE_IPV4:
    case INETADDRESSTYPE_IPV6:
        break;

    case INETADDRESSTYPE_UNKNOWN:
    case INETADDRESSTYPE_IPV4Z:
    case INETADDRESSTYPE_IPV6Z:
    case INETADDRESSTYPE_DNS:
        /*
         * fall through 
         */

    default:
        return MFD_ERROR;
    }

    return MFD_SUCCESS;         /* inetCidrRouteNextHopType index ok */
}                               /* inetCidrRouteNextHopType_check_index */

/*---------------------------------------------------------------------
 * IP-FORWARD-MIB::inetCidrRouteEntry.inetCidrRouteNextHop
 * inetCidrRouteNextHop is subid 6 of inetCidrRouteEntry.
 * Its status is Current, and its access level is NoAccess.
 * OID: .1.3.6.1.2.1.4.24.7.1.6
 * Description:
On remote routes, the address of the next system en              
                route.  For non-remote routes, a zero length string. 

                The type of this address is determined by the value of 
                the inetCidrRouteNextHopType object.
 *
 * Attributes:
 *   accessible 0     isscalar 0     enums  0      hasdefval 0
 *   readable   0     iscolumn 1     ranges 1      hashint   0
 *   settable   0
 *
 * Ranges:  0 - 255;
 *
 * Its syntax is InetAddress (based on perltype OCTETSTR)
 * The net-snmp type is ASN_OCTET_STR. The C type decl is char (char)
 * This data type requires a length.  (Max 255)
 *
 *
 *
 * NOTE: NODE inetCidrRouteNextHop IS NOT ACCESSIBLE
 *
 *
 */
/**
 * check validity of inetCidrRouteNextHop index portion
 *
 * @retval MFD_SUCCESS   : the incoming value is legal
 * @retval MFD_ERROR     : the incoming value is NOT legal
 *
 * @note this is not the place to do any checks for the sanity
 *       of multiple indexes. Those types of checks should be done in the
 *       inetCidrRouteTable_validate_index() function.
 *
 * @note Also keep in mind that if the index refers to a row in this or
 *       some other table, you can't check for that row here to make
 *       decisions, since that row might not be created yet, but may
 *       be created during the processing this request. If you have
 *       such checks, they should be done in the check_dependencies
 *       function, because any new/deleted/changed rows should be
 *       available then.
 *
 * The following checks have already been done for you:
 *    The length is in (one of) the range set(s):  0 - 255
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 */
int
inetCidrRouteNextHop_check_index(inetCidrRouteTable_rowreq_ctx *
                                 rowreq_ctx)
{
    DEBUGMSGTL(("verbose:inetCidrRouteTable:inetCidrRouteNextHop_check_index", "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:426:M: |-> Check inetCidrRouteTable index inetCidrRouteNextHop.
     * check that index value in the table context is legal.
     * (rowreq_ctx->tbl_index.inetCidrRouteNextHop)
     */

    return MFD_SUCCESS;         /* inetCidrRouteNextHop index ok */
}                               /* inetCidrRouteNextHop_check_index */

/**
 * verify specified index is valid.
 *
 * This check is independent of whether or not the values specified for
 * the columns of the new row are valid. Column values and row consistency
 * will be checked later. At this point, only the index values should be
 * checked.
 *
 * All of the individual index validation functions have been called, so this
 * is the place to make sure they are valid as a whole when combined. If
 * you only have one index, then you probably don't need to do anything else
 * here.
 * 
 * @note Keep in mind that if the indexes refer to a row in this or
 *       some other table, you can't check for that row here to make
 *       decisions, since that row might not be created yet, but may
 *       be created during the processing this request. If you have
 *       such checks, they should be done in the check_dependencies
 *       function, because any new/deleted/changed rows should be
 *       available then.
 *
 *
 * @param inetCidrRouteTable_reg
 *        Pointer to the user registration data
 * @param rowreq_ctx
 *        Pointer to the users context.
 * @retval MFD_SUCCESS            : success
 * @retval MFD_CANNOT_CREATE_NOW  : index not valid right now
 * @retval MFD_CANNOT_CREATE_EVER : index never valid
 */
int
inetCidrRouteTable_validate_index(inetCidrRouteTable_registration *
                                  inetCidrRouteTable_reg,
                                  inetCidrRouteTable_rowreq_ctx *
                                  rowreq_ctx)
{
    int             rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:inetCidrRouteTable:inetCidrRouteTable_validate_index", "called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:430:M: |-> Validate potential inetCidrRouteTable index.
     */
    if (rowreq_ctx->tbl_idx.inetCidrRouteDestType !=
        rowreq_ctx->tbl_idx.inetCidrRouteNextHopType) {
        DEBUGMSGTL(("inetCidrRouteTable:validate_index",
                    "ipv4/v6 cross routing not supported\n"));
        return MFD_CANNOT_CREATE_EVER;
    }

    /*
     * InetAddress
     */
    if (((INETADDRESSTYPE_IPV4 ==
          rowreq_ctx->tbl_idx.inetCidrRouteDestType)
         && (4 != rowreq_ctx->tbl_idx.inetCidrRouteDest_len))
        ||
        ((INETADDRESSTYPE_IPV6 ==
          rowreq_ctx->tbl_idx.inetCidrRouteDestType)
         && (16 != rowreq_ctx->tbl_idx.inetCidrRouteDest_len))) {
        DEBUGMSGTL(("inetCidrRouteTable:validate_index",
                    "dest addr type/size mismatch\n"));
        return MFD_CANNOT_CREATE_EVER;
    }
    /*
     * InetAddress
     */
    if (((INETADDRESSTYPE_IPV4 ==
          rowreq_ctx->tbl_idx.inetCidrRouteNextHopType)
         && (4 != rowreq_ctx->tbl_idx.inetCidrRouteNextHop_len))
        ||
        ((INETADDRESSTYPE_IPV6 ==
          rowreq_ctx->tbl_idx.inetCidrRouteNextHopType)
         && (16 != rowreq_ctx->tbl_idx.inetCidrRouteNextHop_len))) {
        DEBUGMSGTL(("inetCidrRouteTable:validate_index",
                    "next hop addr type/size mismatch\n"));
        return MFD_CANNOT_CREATE_EVER;
    }

    return rc;
}                               /* inetCidrRouteTable_validate_index */

/** @} */
