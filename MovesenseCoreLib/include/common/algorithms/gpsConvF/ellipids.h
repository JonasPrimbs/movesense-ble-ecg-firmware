#ifndef _ELLIP_IDS_H_
#define _ELLIP_IDS_H_

#undef IN

typedef enum
{
    AA = 0, // Airy
    AN,     // Australian National
    BR,     // Bessel 1841. Ethiopia, Indonesia, Japan and Korea
    BN,     // Bessel 1841. Namibia
    CC,     // Clarke 1866
    CD,     // Clarke 1880
    EB,     // Everest. Brunei and East Malaysia (Sabah and Sarawak)
    EA,     // Everest. India 1830
    EC,     // Everest. India 1956
    EF,     // Everest. Pakistan
    EE,     // Everest. West Malaysia and Singapore 1948
    ED,     // Everest. West Malaysia 1969
    RF,     // GRS 1980. Geodetic Reference System 1980
    HE,     // Helmert 1906
    HO,     // Hough 1960
    ID,     // Indonesian 1974
    IN,     // International 1924
    KA,     // Krassovsky 1940
    AM,     // Modified Airy
    FA,     // Modified Fischer 1960
    SA,     // South American 1969
    WD,     // WGS 1972
    WE,     // WGS 1984
    GR      // GRS 1967
} ELLIP_ID;

#endif
