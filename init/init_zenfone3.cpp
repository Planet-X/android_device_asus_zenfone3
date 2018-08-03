/*
   Copyright (c) 2014, The Linux Foundation. All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.
   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <sys/stat.h>

#include <android-base/logging.h>
#include <android-base/properties.h>
#include "property_service.h"

#include <fcntl.h>
#include <unistd.h>

namespace android {
namespace init {

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

static void set_serial()
{
    int fd, rc;
    char buf[16];
    int status = 1;
    const char *path = "/factory/SSN";

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        status = -1;
    }

    if (rc = read(fd, buf, 15) < 0) {
        status = -1;
    } else {
        buf[15] = '\0';
        property_override("ro.serialno", buf);
    }
    close(fd);

    if (status < 0) {
        property_override("ro.serialno", "UNKNOWNSERIALNO");
    }
}

const char * get_variant(int project)
{
    int rf = stoi(android::base::GetProperty("ro.boot.id.rf", ""));

    if (project == 6) {
        switch(rf){
            case 0: return "ASUS_Z017D";  /* Global Varient */
            case 1: return "ASUS_Z017DB"; /* Indonesian Varient */
            case 2: return "ASUS_Z017DC"; /* Latin American Varient */
            default: return "ASUS_Z017DA"; /* Default to Z017DA */
        }
    } else {
        switch(rf){
            case 0: return "ASUS_Z012S";  /* Canadian varient */
            case 1: return "ASUS_Z012DB"; /* Indonesian Varient */
            case 2: return "ASUS_Z012DC"; /* Latin American variant */
            case 8: return "ASUS_Z012DA";  /* JP/TW variant */
            case 15: return "ASUS_Z012DE"; /* Chinese Varient */
            default: return "ASUS_Z012DA"; /* Default to Z012DA */
        }
    }
}

void vendor_load_properties()
{
    set_serial();

    int project = stoi(android::base::GetProperty("ro.boot.id.prj", ""));
    const char * model = get_variant(project);
    
    property_set("ro.build.model", model);
    property_set("ro.product.model", model);
    property_override("ro.product.name", "WW_Phone");
    
    if (project == 6) {
        property_set("ro.build.device", "Z017");
        property_override("ro.product.device", "ASUS_Z017D_1");
        
    } else {
        property_set("ro.build.device", "Z012");
        property_override("ro.product.device", "ASUS_Z012D");
    }
}
}
}
