/*
 *------------------------------------------------------------------
 * sdp_config.c  -- Application configuration routines for the
 *                  generic SDP parser.
 *
 * March 2001, D. Renee Revis
 *
 * Copyright (c) 2001, 2004-2005 by cisco Systems, Inc.
 * All rights reserved.
 *------------------------------------------------------------------
 */

#include "sdp_os_defs.h"
#include "sdp.h"
#include "sdp_private.h"


/* Function:    sdp_verify_conf_ptr
 * Description: Verify the configuration pointer is valid by checking for
 *              the SDP magic number.  If not valid, display an error.
 *              Note that we can't keep a statistic of this because we
 *              track stats inside the config structure.
 * Parameters:	conf_p    The configuration structure handle.
 *              str       String containing function name to print.
 * Returns:	TRUE or FALSE.
 */
tinybool sdp_verify_conf_ptr (sdp_conf_options_t *conf_p) 
{
    if ((conf_p != NULL) && (conf_p->magic_num == SDP_MAGIC_NUM)) {
        return (TRUE);
    } else {
        sdp_log_errmsg(SDP_ERR_INVALID_CONF_PTR, NULL);
        return (FALSE);
    }
}

/* Function:    void *sdp_init_config()
 * Description: Initialize SDP configuration structure with the
 *              following defaults:
 *              All debug levels turned OFF.
 *              All token lines required per RFC2327.
 *              No media types supported.
 *              No network types supported.
 *              No address types supported.
 *              No transport types supported.
 * Parameters:	None.
 * Returns:	A handle for the configuration as a void ptr.
 */
void *sdp_init_config ()
{
    int i;
    sdp_conf_options_t *conf_p;

    conf_p = (sdp_conf_options_t *)SDP_MALLOC(sizeof(sdp_conf_options_t));
    if (conf_p == NULL) {
        return (NULL);
    }

    /* Initialize magic number. */
    conf_p->magic_num = SDP_MAGIC_NUM;

    /* Set default debug flags. */
    conf_p->debug_flag[SDP_DEBUG_TRACE]    = FALSE;
    conf_p->debug_flag[SDP_DEBUG_WARNINGS] = FALSE;
    conf_p->debug_flag[SDP_DEBUG_ERRORS]   = FALSE;

    /* Set required lines flags.  Note: Only need to set those that */
    /* are questionable.  Most lines aren't required by default.    */
    conf_p->version_reqd       = TRUE;
    conf_p->owner_reqd         = TRUE;
    conf_p->session_name_reqd  = TRUE;
    conf_p->timespec_reqd      = TRUE;

    /* No media types supported by default. */
    for (i=0; i < SDP_MAX_MEDIA_TYPES; i++) {
        conf_p->media_supported[i] = FALSE;
    }

    /* No network types supported by default. */
    for (i=0; i < SDP_MAX_NETWORK_TYPES; i++) {
        conf_p->nettype_supported[i] = FALSE;
    }

    /* No address types supported by default. */
    for (i=0; i < SDP_MAX_ADDR_TYPES; i++) {
        conf_p->addrtype_supported[i] = FALSE;
    }

    /* No transport types supported by default. */
    for (i=0; i < SDP_MAX_TRANSPORT_TYPES; i++) {
        conf_p->transport_supported[i] = FALSE;
    }

    /* No choose parameters allowed by default. */
    for (i=0; i < SDP_MAX_CHOOSE_PARAMS; i++) {
        conf_p->allow_choose[i] = FALSE;
    }

    /* Initialize statistics counts */
    conf_p->num_parses              = 0;
    conf_p->num_builds              = 0;
    conf_p->num_not_sdp_desc        = 0;
    conf_p->num_invalid_token_order = 0;
    conf_p->num_invalid_param       = 0;
    conf_p->num_no_resource         = 0;

    return (conf_p);
}


/* Function:    void sdp_appl_debug(void *config_p, sdp_debug_e debug_type,
 *                                  tinybool my_bool);
 * Description:	Define the default type of debug for the application.  
 *              Valid debug types are ERRORS, WARNINGS, and TRACE.  Each 
 *              debug type can be turned on/off individually.  The 
 *              default debug level can be redefined at any time.
 * Parameters:	conf_p     The config handle returned by sdp_init_config.
 *              debug_type Specifies the debug type being enabled/disabled.
 *              debug_flag  Defines whether the debug should be enabled or not.
 * Returns:	Nothing.
 */
void sdp_appl_debug (void *config_p, sdp_debug_e debug_type, 
                     tinybool debug_flag)
{
    sdp_conf_options_t *conf_p = (sdp_conf_options_t *)config_p;

    if (sdp_verify_conf_ptr(conf_p) == FALSE) {
        return;
    }

    conf_p->debug_flag[debug_type] = debug_flag;
}


/* Functions:	void sdp_require_version
 *              void sdp_require_owner
 *              void sdp_require_session_name
 *              void sdp_require_timespec
 * Description:	These functions allow the application to not require several 
 *              of the tokens that are specifically required by RFC 2327.
 * Parameters:	conf_p    The config handle returned by sdp_init_config.
 *              version_required   TRUE or FALSE whether the token should 
 *              be required.
 * Returns:	Nothing.
 */
void sdp_require_version (void *config_p, tinybool version_required)
{
    sdp_conf_options_t *conf_p = (sdp_conf_options_t *)config_p;

    if (sdp_verify_conf_ptr(conf_p) == FALSE) {
        return;
    }

    conf_p->version_reqd = version_required;
}

void sdp_require_owner (void *config_p, tinybool owner_required)
{
    sdp_conf_options_t *conf_p = (sdp_conf_options_t *)config_p;

    if (sdp_verify_conf_ptr(conf_p) == FALSE) {
        return;
    }

    conf_p->owner_reqd = owner_required;
}

void sdp_require_session_name (void *config_p, tinybool sess_name_required)
{
    sdp_conf_options_t *conf_p = (sdp_conf_options_t *)config_p;

    if (sdp_verify_conf_ptr(conf_p) == FALSE) {
        return;
    }

    conf_p->session_name_reqd = sess_name_required;
}

void sdp_require_timespec (void *config_p, tinybool timespec_required)
{
    sdp_conf_options_t *conf_p = (sdp_conf_options_t *)config_p;

    if (sdp_verify_conf_ptr(conf_p) == FALSE) {
        return;
    }

    conf_p->timespec_reqd = timespec_required;
}


/* Function:    sdp_media_supported
 * Description: These functions allow the application to specify which 
 *              media types it supports. The application must set any/all 
 *              as required.  No media types are supported by default.
 * Parameters:  config_p    The config handle returned by sdp_init_config.
 *              nettype     The network type for which support is being set.
 *              media_supported TRUE or FALSE whether the support is provided.
 * Returns:     Nothing.
 */
void sdp_media_supported (void *config_p, sdp_media_e media_type, 
			 tinybool media_supported)
{
    sdp_conf_options_t *conf_p = (sdp_conf_options_t *)config_p;

    if (sdp_verify_conf_ptr(conf_p) == FALSE) {
        return;
    }

    conf_p->media_supported[media_type] = media_supported;
}


/* Function:    sdp_nettype_supported
 * Description: This function allows the application to specify which 
 *              network types it supports.  The application must set 
 *              any/all as required.  No network types are supported by
 *              default.
 * Parameters:  config_p    The config handle returned by sdp_init_config.
 *              nettype     The network type for which support is being set.
 *              nettype_supported TRUE or FALSE whether the support is 
 *                          provided.
 * Returns:     Nothing.
 */
void sdp_nettype_supported (void *config_p, sdp_nettype_e nettype, 
			    tinybool nettype_supported)
{
    sdp_conf_options_t *conf_p = (sdp_conf_options_t *)config_p;

    if (sdp_verify_conf_ptr(conf_p) == FALSE) {
        return;
    }

    conf_p->nettype_supported[nettype] = nettype_supported;
}


/* Function:    sdp_addrtype_supported
 * Description: This function allows the application to specify which 
 *              address types it supports.  The application must set 
 *              any/all as required.  No address types are supported by
 *              default.
 * Parameters:  config_p    The config handle returned by sdp_init_config.
 *              addrtype    The address type for which support is being set.
 *              addrtype_supported TRUE or FALSE whether the support is 
 *                          provided.
 * Returns:     Nothing.
 */
void sdp_addrtype_supported (void *config_p, sdp_addrtype_e addrtype, 
			     tinybool addrtype_supported)
{
    sdp_conf_options_t *conf_p = (sdp_conf_options_t *)config_p;

    if (sdp_verify_conf_ptr(conf_p) == FALSE) {
        return;
    }

    conf_p->addrtype_supported[addrtype] = addrtype_supported;
}


/* Function:    sdp_transport_supported
 * Description: This function allows the application to specify which 
 *              transport types it supports.  The application must set 
 *              any/all as required.  No transport types are supported
 *              by default.
 * Parameters:  config_p    The config handle returned by sdp_init_config.
 *              transport   The transport type for which support is being set.
 *              transport_supported  TRUE or FALSE whether the support is 
 *                          provided.
 * Returns:     Nothing.
 */
void sdp_transport_supported (void *config_p, sdp_transport_e transport, 
			      tinybool transport_supported)
{
    sdp_conf_options_t *conf_p = (sdp_conf_options_t *)config_p;

    if (sdp_verify_conf_ptr(conf_p) == FALSE) {
        return;
    }

    conf_p->transport_supported[transport] = transport_supported;
}


/* Function:    sdp_allow_choose
 * Description: These functions allow the CHOOSE parameter `$' to be 
 *              specified in place of certain parameters.
 * Parameters:  config_p        The config handle returned by sdp_init_config.
 *              param           The param that may or may not be CHOOSE.
 *              choose_allowed  TRUE or FALSE whether the CHOOSE parameter 
 *                              should be allowed.
 * Returns:     Nothing.
 */
void sdp_allow_choose (void *config_p, sdp_choose_param_e param, tinybool choose_allowed)
{
    sdp_conf_options_t *conf_p = (sdp_conf_options_t *)config_p;

    if (sdp_verify_conf_ptr(conf_p) == FALSE) {
        return;
    }

    conf_p->allow_choose[param] = choose_allowed;
}


/* Function:    sdp_show_config
 * Description: This function will display the current configuration
 *              defined for the handle specified. We sprintf into a buffer
 *              and then print the buffer because IOS can swap out on a 
 *              print and the conf_p pointer may no longer be valid.
 * Parameters:  config_p    The config handle returned by sdp_init_config.
 * Returns:     Nothing.
 */
void sdp_show_config (void *config_p)
{
    int                 i;
    char                buffer[500];
    char               *p;
    tinybool            any_supported;
    sdp_conf_options_t *conf_p = (sdp_conf_options_t *)config_p;

    if (sdp_verify_conf_ptr(conf_p) == FALSE) {
        return;
    }

    p = buffer;
    p += sprintf(p, "\nSDP application configuration:");

    if (conf_p->version_reqd == TRUE) {
        p += sprintf(p, "\n Version line (v=) required");
    }
    if (conf_p->owner_reqd == TRUE) {
        p += sprintf(p, "\n Owner line (o=) required");
    }
    if (conf_p->session_name_reqd == TRUE) {
        p += sprintf(p, "\n Session name line (s=) required");
    }
    if (conf_p->timespec_reqd == TRUE) {
        p += sprintf(p, "\n Timespec line (t=) required");
    }

    /* Show supported media types. */
    any_supported = FALSE;
    p += sprintf(p, "\n Media supported: ");
    for (i=0; i < SDP_MAX_MEDIA_TYPES; i++) {
        if (conf_p->media_supported[i] == TRUE) {
            p += sprintf(p, "%s ", sdp_media[i].name);
            any_supported = TRUE;
            }
    }
    if (any_supported == FALSE) {
        p += sprintf(p, "none");
    }

    /* Show supported network types. */
    any_supported = FALSE;
    p += sprintf(p, "\n Network types supported: ");
    for (i=0; i < SDP_MAX_NETWORK_TYPES; i++) {
        if (conf_p->nettype_supported[i] == TRUE) {
            p += sprintf(p, "%s ", sdp_nettype[i].name);
            any_supported = TRUE;
        }
    }
    if (any_supported == FALSE) {
        p += sprintf(p, "none");
    }

    /* Show supported address types. */
    any_supported = FALSE;
    p += sprintf(p, "\n Address types supported: ");
    for (i=0; i < SDP_MAX_ADDR_TYPES; i++) {
        if (conf_p->addrtype_supported[i] == TRUE) {
            p += sprintf(p, "%s ", sdp_addrtype[i].name);
            any_supported = TRUE;
        }
    }
    if (any_supported == FALSE) {
        p += sprintf(p, "none");
    }

    /* Show supported transport types. */
    any_supported = FALSE;
    p += sprintf(p, "\n Transport types supported: ");
    for (i=0; i < SDP_MAX_NETWORK_TYPES; i++) {
        if (conf_p->transport_supported[i] == TRUE) {
            p += sprintf(p, "%s ", sdp_transport[i].name);
            any_supported = TRUE;
        }
    }
    if (any_supported == FALSE) {
        p += sprintf(p, "none");
    }
    p += sprintf(p, "\n");

    printf("%s\n", buffer);
}


/* Function:    sdp_show_stats
 * Description: This function will display the current statistics
 *              defined for the handle specified. We sprintf into a buffer
 *              and then print the buffer because IOS can swap out on a 
 *              print and the conf_p pointer may no longer be valid.
 * Parameters:  config_p    The config handle returned by sdp_init_config.
 * Returns:     Nothing.
 */
void sdp_show_stats (void *config_p)
{
    char                buffer[200];
    char               *p;
    sdp_conf_options_t *conf_p = (sdp_conf_options_t *)config_p;

    if (sdp_verify_conf_ptr(conf_p) == FALSE) {
        return;
    }

    p = buffer;
    p += sprintf(p, "\nSDP application statistics:");
    p += sprintf(p, "\n Parses: %lu,  Builds %lu", 
              conf_p->num_parses,conf_p->num_builds);
    p += sprintf(p, "\n Invalid token order: %lu,  Invalid param: %lu",
              conf_p->num_invalid_token_order, conf_p->num_invalid_param);
    p += sprintf(p, "\n Not SDP desc: %lu,  No resource: %lu",
              conf_p->num_not_sdp_desc, conf_p->num_no_resource);

    printf("%s\n", buffer);
}

/* Function:    sdp_clear_stats
 * Description: This function will clear the current statistics
 *              defined for the handle specified.
 * Parameters:  config_p    The config handle returned by sdp_init_config.
 * Returns:     Nothing.
 */
void sdp_clear_stats (void *config_p)
{
    sdp_conf_options_t *conf_p = (sdp_conf_options_t *)config_p;

    if (sdp_verify_conf_ptr(conf_p) == FALSE) {
        return;
    }

    conf_p->num_parses              = 0;
    conf_p->num_builds              = 0;
    conf_p->num_not_sdp_desc        = 0;
    conf_p->num_invalid_token_order = 0;
    conf_p->num_invalid_param       = 0;
    conf_p->num_no_resource         = 0;
}
