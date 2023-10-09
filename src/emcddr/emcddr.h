#pragma once

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

enum DdrType
{
    DDR_TYPE_32MB,
    DDR_TYPE_64MB
};

void init_ddr(enum DdrType type);

#ifdef __cplusplus
}
#endif //__cplusplus
