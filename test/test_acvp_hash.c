/** @file */
/*****************************************************************************
* Copyright (c) 2019, Cisco Systems, Inc.
* All rights reserved.

* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include "ut_common.h"
#include "acvp_lcl.h"

static ACVP_CTX *ctx = NULL;
static ACVP_RESULT rv = 0;
static JSON_Object *obj = NULL;
static JSON_Value *val = NULL;

static void setup(void) {
    setup_empty_ctx(&ctx);

    rv = acvp_cap_hash_enable(ctx, ACVP_HASH_SHA256, &dummy_handler_success);
    cr_assert(rv == ACVP_SUCCESS);
    rv = acvp_cap_hash_set_parm(ctx, ACVP_HASH_SHA256, ACVP_HASH_IN_BIT, 0);
    cr_assert(rv == ACVP_SUCCESS);
    rv = acvp_cap_hash_set_parm(ctx, ACVP_HASH_SHA256, ACVP_HASH_IN_EMPTY, 1);
    cr_assert(rv == ACVP_SUCCESS);
}

static void fail_setup(void) {
    setup_empty_ctx(&ctx);

    rv = acvp_cap_hash_enable(ctx, ACVP_HASH_SHA256, &dummy_handler_failure);
    cr_assert(rv == ACVP_SUCCESS);
    rv = acvp_cap_hash_set_parm(ctx, ACVP_HASH_SHA256, ACVP_HASH_IN_BIT, 0);
    cr_assert(rv == ACVP_SUCCESS);
    rv = acvp_cap_hash_set_parm(ctx, ACVP_HASH_SHA256, ACVP_HASH_IN_EMPTY, 1);
    cr_assert(rv == ACVP_SUCCESS);
}

static void teardown(void) {
    if (ctx) teardown_ctx(&ctx);
}

/*
 * Test capabilites API.
 */
Test(HASH_CAPABILITY, good) {
    setup_empty_ctx(&ctx);

    rv = acvp_cap_hash_enable(ctx, ACVP_HASH_SHA1, &dummy_handler_success);
    cr_assert(rv == ACVP_SUCCESS);
    rv = acvp_cap_hash_set_parm(ctx, ACVP_HASH_SHA1, ACVP_HASH_IN_BIT, 0);
    cr_assert(rv == ACVP_SUCCESS);
    rv = acvp_cap_hash_set_parm(ctx, ACVP_HASH_SHA1, ACVP_HASH_IN_EMPTY, 1);
    cr_assert(rv == ACVP_SUCCESS);

    rv = acvp_cap_hash_enable(ctx, ACVP_HASH_SHA224, &dummy_handler_success);
    cr_assert(rv == ACVP_SUCCESS);
    rv = acvp_cap_hash_set_parm(ctx, ACVP_HASH_SHA224, ACVP_HASH_IN_BIT, 0);
    cr_assert(rv == ACVP_SUCCESS);
    rv = acvp_cap_hash_set_parm(ctx, ACVP_HASH_SHA224, ACVP_HASH_IN_EMPTY, 1);
    cr_assert(rv == ACVP_SUCCESS);

    rv = acvp_cap_hash_enable(ctx, ACVP_HASH_SHA256, &dummy_handler_success);
    cr_assert(rv == ACVP_SUCCESS);
    rv = acvp_cap_hash_set_parm(ctx, ACVP_HASH_SHA256, ACVP_HASH_IN_BIT, 0);
    cr_assert(rv == ACVP_SUCCESS);
    rv = acvp_cap_hash_set_parm(ctx, ACVP_HASH_SHA256, ACVP_HASH_IN_EMPTY, 1);
    cr_assert(rv == ACVP_SUCCESS);

    rv = acvp_cap_hash_enable(ctx, ACVP_HASH_SHA384, &dummy_handler_success);
    cr_assert(rv == ACVP_SUCCESS);
    rv = acvp_cap_hash_set_parm(ctx, ACVP_HASH_SHA384, ACVP_HASH_IN_BIT, 0);
    cr_assert(rv == ACVP_SUCCESS);
    rv = acvp_cap_hash_set_parm(ctx, ACVP_HASH_SHA384, ACVP_HASH_IN_EMPTY, 1);
    cr_assert(rv == ACVP_SUCCESS);

    rv = acvp_cap_hash_enable(ctx, ACVP_HASH_SHA512, &dummy_handler_success);
    cr_assert(rv == ACVP_SUCCESS);
    rv = acvp_cap_hash_set_parm(ctx, ACVP_HASH_SHA512, ACVP_HASH_IN_BIT, 0);
    cr_assert(rv == ACVP_SUCCESS);
    rv = acvp_cap_hash_set_parm(ctx, ACVP_HASH_SHA512, ACVP_HASH_IN_EMPTY, 1);
    cr_assert(rv == ACVP_SUCCESS);

    teardown_ctx(&ctx);
}

/*
 * Test the KAT handler API.
 * The ctx is empty (no capabilities), expecting failure.
 */
Test(HASH_API, empty_ctx) {
    setup_empty_ctx(&ctx);

    val = json_parse_file("json/hash/hash.json");

    obj = ut_get_obj_from_rsp(val);
    if (!obj) {
        ACVP_LOG_ERR("JSON obj parse error");
        goto end;
    }

    rv  = acvp_hash_kat_handler(ctx, obj);
    cr_assert(rv == ACVP_UNSUPPORTED_OP);
    json_value_free(val);

end:
    if (ctx) teardown_ctx(&ctx);
}

/*
 * Test KAT handler API.
 * The ctx is NULL, expecting failure.
 */
Test(HASH_API, null_ctx) {
    val = json_parse_file("json/hash/hash.json");

    obj = ut_get_obj_from_rsp(val);
    if (!obj) {
        ACVP_LOG_ERR("JSON obj parse error");
        return;
    }

    /* Test with NULL JSON object */
    rv  = acvp_hash_kat_handler(NULL, obj);
    cr_assert(rv == ACVP_NO_CTX);
    json_value_free(val);
}


/*
 * Test the KAT handler API.
 * The obj is null, expecting failure.
 */
Test(HASH_API, null_json_obj, .init = setup, .fini = teardown) {
    rv  = acvp_hash_kat_handler(ctx, NULL);
    cr_assert(rv == ACVP_MALFORMED_JSON);
}

/*
 * This is a good JSON.
 * Expecting success.
 */
Test(HASH_HANDLER, good, .init = setup, .fini = teardown) {
    val = json_parse_file("json/hash/hash.json");

    obj = ut_get_obj_from_rsp(val);
    if (!obj) {
        ACVP_LOG_ERR("JSON obj parse error");
        return;
    }
    rv = acvp_hash_kat_handler(ctx, obj);
    cr_assert(rv == ACVP_SUCCESS);
    json_value_free(val);
}


/*
 * The value for key:"algorithm" is wrong.
 */
Test(HASH_HANDLER, wrong_algorithm, .init = setup, .fini = teardown) {
    val = json_parse_file("json/hash/hash_1.json");

    obj = ut_get_obj_from_rsp(val);
    if (!obj) {
        ACVP_LOG_ERR("JSON obj parse error");
        return;
    }
    rv = acvp_hash_kat_handler(ctx, obj);
    cr_assert(rv == ACVP_UNSUPPORTED_OP);
    json_value_free(val);
}


/*
 * The key:"testType" is missing.
 */
Test(HASH_HANDLER, missing_testType, .init = setup, .fini = teardown) {
    val = json_parse_file("json/hash/hash_2.json");

    obj = ut_get_obj_from_rsp(val);
    if (!obj) {
        ACVP_LOG_ERR("JSON obj parse error");
        return;
    }
    rv = acvp_hash_kat_handler(ctx, obj);
    cr_assert(rv == ACVP_MISSING_ARG);
    json_value_free(val);
}


/*
 * The value for key:"testType" is wrong.
 */
Test(HASH_HANDLER, wrong_testType, .init = setup, .fini = teardown) {
    val = json_parse_file("json/hash/hash_3.json");

    obj = ut_get_obj_from_rsp(val);
    if (!obj) {
        ACVP_LOG_ERR("JSON obj parse error");
        return;
    }
    rv = acvp_hash_kat_handler(ctx, obj);
    cr_assert(rv == ACVP_INVALID_ARG);
    json_value_free(val);
}


/*
 * The key:"msg" is missing.
 */
Test(HASH_HANDLER, missing_msg, .init = setup, .fini = teardown) {
    val = json_parse_file("json/hash/hash_4.json");

    obj = ut_get_obj_from_rsp(val);
    if (!obj) {
        ACVP_LOG_ERR("JSON obj parse error");
        return;
    }
    rv = acvp_hash_kat_handler(ctx, obj);
    cr_assert(rv == ACVP_MISSING_ARG);
    json_value_free(val);
}


/*
 * The value for key:"msg" string is too long.
 */
Test(HASH_HANDLER, long_msg, .init = setup, .fini = teardown) {
    val = json_parse_file("json/hash/hash_5.json");

    obj = ut_get_obj_from_rsp(val);
    if (!obj) {
        ACVP_LOG_ERR("JSON obj parse error");
        return;
    }
    rv = acvp_hash_kat_handler(ctx, obj);
    cr_assert(rv == ACVP_INVALID_ARG);
    json_value_free(val);
}


/*
 * The value for key:"tgId" is missing
 */
Test(HASH_HANDLER, missing_tgId, .init = setup, .fini = teardown) {
    val = json_parse_file("json/hash/hash_6.json");
    
    obj = ut_get_obj_from_rsp(val);
    if (!obj) {
        ACVP_LOG_ERR("JSON obj parse error");
        return;
    }
    rv = acvp_hash_kat_handler(ctx, obj);
    cr_assert(rv == ACVP_MALFORMED_JSON);
    json_value_free(val);
}

/*
 * The value for key:"testType" is missing in the last test_group
 */
Test(HASH_HANDLER, missing_lasttgId, .init = setup, .fini = teardown) {
    val = json_parse_file("json/hash/hash_7.json");
    
    obj = ut_get_obj_from_rsp(val);
    if (!obj) {
        ACVP_LOG_ERR("JSON obj parse error");
        return;
    }
    rv = acvp_hash_kat_handler(ctx, obj);
    cr_assert(rv == ACVP_MISSING_ARG);
    json_value_free(val);
}

/*
 * The value for key:last AFT tcId "msg" is missing
 */
Test(HASH_HANDLER, missing_lasttcId, .init = setup, .fini = teardown) {
    val = json_parse_file("json/hash/hash_8.json");
    
    obj = ut_get_obj_from_rsp(val);
    if (!obj) {
        ACVP_LOG_ERR("JSON obj parse error");
        return;
    }
    rv = acvp_hash_kat_handler(ctx, obj);
    cr_assert(rv == ACVP_MISSING_ARG);
    json_value_free(val);
}

/*
 * This is a good JSON with failing crypto handler.
 * Will fail as defined by the counter values.
 */
Test(HASH_HANDLER, cryptoFail1, .init = fail_setup, .fini = teardown) {
    val = json_parse_file("json/hash/hash.json");

    obj = ut_get_obj_from_rsp(val);
    if (!obj) {
        ACVP_LOG_ERR("JSON obj parse error");
        return;
    }
    counter_set = 0;
    counter_fail = 0; /* fail on first iteration of AFT */
    rv = acvp_hash_kat_handler(ctx, obj);
    cr_assert(rv == ACVP_CRYPTO_MODULE_FAIL);
    json_value_free(val);
}

/*
 * This is a good JSON with failing crypto handler.
 * Will fail as defined by the counter values.
 */
Test(HASH_HANDLER, cryptoFail2, .init = fail_setup, .fini = teardown) {
    val = json_parse_file("json/hash/hash.json");

    obj = ut_get_obj_from_rsp(val);
    if (!obj) {
        ACVP_LOG_ERR("JSON obj parse error");
        return;
    }
    counter_set = 0;
    counter_fail = 5;  /* fail on 6th iteration of AFT */
    rv = acvp_hash_kat_handler(ctx, obj);
    cr_assert(rv == ACVP_CRYPTO_MODULE_FAIL);
    json_value_free(val);
}

/*
 * This is a good JSON with failing crypto handler.
 * Will fail as defined by the counter values.
 */
Test(HASH_HANDLER, cryptoFail3, .init = fail_setup, .fini = teardown) {
    val = json_parse_file("json/hash/hash.json");

    obj = ut_get_obj_from_rsp(val);
    if (!obj) {
        ACVP_LOG_ERR("JSON obj parse error");
        return;
    }
    counter_set = 0;
    counter_fail = 129; /* fail on first iteration of MCT */
    rv = acvp_hash_kat_handler(ctx, obj);
    cr_assert(rv == ACVP_CRYPTO_MODULE_FAIL);
    json_value_free(val);
}

/*
 * This is a good JSON with failing crypto handler.
 * Will fail as defined by the counter values.
 */
Test(HASH_HANDLER, cryptoFail4, .init = fail_setup, .fini = teardown) {
    val = json_parse_file("json/hash/hash.json");

    obj = ut_get_obj_from_rsp(val);
    if (!obj) {
        ACVP_LOG_ERR("JSON obj parse error");
        return;
    }
    counter_set = 0;
    counter_fail = 135;  /* fail on 6th iteration of MCT */
    rv = acvp_hash_kat_handler(ctx, obj);
    cr_assert(rv == ACVP_CRYPTO_MODULE_FAIL);
    json_value_free(val);
}


