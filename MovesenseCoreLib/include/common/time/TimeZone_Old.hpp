#pragma once
/**
        @file TimeZone.hpp
        Timezone information interface

        @note
        Data from ambit project (timezones.sql)
        This was the table was used for the firmware till 1.1.34
        As old firmware has old timezone ids, so we need to keep the old table to convert it to new ids
*/

#include <cstdint>
#include <cstddef>
#include "common/time/TimeZone.hpp"

namespace nea
{

// X2_( id TEXT(2), subId, name_part_1, empty, name_part_2, offset REAL, dstrule INTEGER )
// X3_( id TEXT(2), subId, name_part_1, name_part_2, name_part_3, offset REAL, dstrule INTEGER )
#define FOR_TIMEZONE_OLD_X_                                                                             \
    X2_(CI, Abidjan, Africa, , Abidjan, 0, 0)                                                      \
    X2_(GH, Accra, Africa, , Accra, 0, 0)                                                          \
    X2_(ET, Addis_Ababa, Africa, , Addis Ababa, 3, 0)                                              \
    X2_(DZ, Algiers, Africa, , Algiers, 1, 0)                                                      \
    X2_(ER, Asmara, Africa, , Asmara, 3, 0)                                                        \
    X2_(ML, Bamako, Africa, , Bamako, 0, 0)                                                        \
    X2_(CF, Bangui, Africa, , Bangui, 1, 0)                                                        \
    X2_(GM, Banjul, Africa, , Banjul, 0, 0)                                                        \
    X2_(GW, Bissau, Africa, , Bissau, 0, 0)                                                        \
    X2_(MW, Blantyre, Africa, , Blantyre, 2, 0)                                                    \
    X2_(CG, Brazzaville, Africa, , Brazzaville, 1, 0)                                              \
    X2_(BI, Bujumbura, Africa, , Bujumbura, 2, 0)                                                  \
    X2_(EG, Cairo, Africa, , Cairo, 2, 0)                                                          \
    X2_(MA, Casablanca, Africa, , Casablanca, 0, 2)                                                \
    X2_(ES, Ceuta, Africa, , Ceuta, 1, 1)                                                          \
    X2_(GN, Conakry, Africa, , Conakry, 0, 0)                                                      \
    X2_(SN, Dakar, Africa, , Dakar, 0, 0)                                                          \
    X2_(TZ, Dar_es_Salaam, Africa, , Dar es Salaam, 3, 0)                                          \
    X2_(DJ, Djibouti, Africa, , Djibouti, 3, 0)                                                    \
    X2_(CM, Douala, Africa, , Douala, 1, 0)                                                        \
    X2_(EH, El_Aaiun, Africa, , El Aaiun, 0, 2)                                                    \
    X2_(SL, Freetown, Africa, , Freetown, 0, 0)                                                    \
    X2_(BW, Gaborone, Africa, , Gaborone, 2, 0)                                                    \
    X2_(ZW, Harare, Africa, , Harare, 2, 0)                                                        \
    X2_(ZA, Johannesburg, Africa, , Johannesburg, 2, 0)                                            \
    X2_(SS, Juba, Africa, , Juba, 3, 0)                                                            \
    X2_(UG, Kampala, Africa, , Kampala, 3, 0)                                                      \
    X2_(SD, Khartoum, Africa, , Khartoum, 3, 0)                                                    \
    X2_(RW, Kigali, Africa, , Kigali, 2, 0)                                                        \
    X2_(CD, Kinshasa, Africa, , Kinshasa, 1, 0)                                                    \
    X2_(NG, Lagos, Africa, , Lagos, 1, 0)                                                          \
    X2_(GA, Libreville, Africa, , Libreville, 1, 0)                                                \
    X2_(TG, Lome, Africa, , Lome, 0, 0)                                                            \
    X2_(AO, Luanda, Africa, , Luanda, 1, 0)                                                        \
    X2_(CD, Lubumbashi, Africa, , Lubumbashi, 2, 0)                                                \
    X2_(ZM, Lusaka, Africa, , Lusaka, 2, 0)                                                        \
    X2_(GQ, Malabo, Africa, , Malabo, 1, 0)                                                        \
    X2_(MZ, Maputo, Africa, , Maputo, 2, 0)                                                        \
    X2_(LS, Maseru, Africa, , Maseru, 2, 0)                                                        \
    X2_(SZ, Mbabane, Africa, , Mbabane, 2, 0)                                                      \
    X2_(SO, Mogadishu, Africa, , Mogadishu, 3, 0)                                                  \
    X2_(LR, Monrovia, Africa, , Monrovia, 0, 0)                                                    \
    X2_(KE, Nairobi, Africa, , Nairobi, 3, 0)                                                      \
    X2_(TD, Ndjamena, Africa, , Ndjamena, 1, 0)                                                    \
    X2_(NE, Niamey, Africa, , Niamey, 1, 0)                                                        \
    X2_(MR, Nouakchott, Africa, , Nouakchott, 0, 0)                                                \
    X2_(BF, Ouagadougou, Africa, , Ouagadougou, 0, 0)                                              \
    X2_(BJ, Porto_Novo, Africa, , Porto - Novo, 1, 0)                                              \
    X2_(ST, Sao_Tome, Africa, , Sao Tome, 0, 0)                                                    \
    X2_(LY, Tripoli, Africa, , Tripoli, 2, 0)                                                      \
    X2_(TN, Tunis, Africa, , Tunis, 1, 0)                                                          \
    X2_(NA, Windhoek, Africa, , Windhoek, 1, 5)                                                    \
    X2_(US, Adak, America, , Adak, -10, 6)                                                         \
    X2_(US, Anchorage, America, , Anchorage, -9, 6)                                                \
    X2_(AI, Anguilla, America, , Anguilla, -4, 0)                                                  \
    X2_(AG, Antigua, America, , Antigua, -4, 0)                                                    \
    X2_(BR, Araguaina, America, , Araguaina, -3, 0)                                                \
    X3_(AR, Buenos_Aires, America, Argentina, Buenos Aires, -3, 0)                                 \
    X3_(AR, Buenos_Aires, America, Argentina, Catamarca, -3, 0)                                       \
    X3_(AR, Cordoba, America, Argentina, Cordoba, -3, 0)                                           \
    X3_(AR, Buenos_Aires, America, Argentina, Jujuy, -3, 0)                                               \
    X3_(AR, Buenos_Aires, America, Argentina, La Rioja, -3, 0)                                         \
    X3_(AR, Mendoza, America, Argentina, Mendoza, -3, 0)                                           \
    X3_(AR, Rio_Gallegos, America, Argentina, Rio Gallegos, -3, 0)                                 \
    X3_(AR, Salta, America, Argentina, Salta, -3, 0)                                               \
    X3_(AR, Buenos_Aires, America, Argentina, San Juan, -3, 0)                                         \
    X3_(AR, Buenos_Aires, America, Argentina, San Luis, -3, 0)                                         \
    X3_(AR, Buenos_Aires, America, Argentina, Tucuman, -3, 0)                                           \
    X3_(AR, Ushuaia, America, Argentina, Ushuaia, -3, 0)                                           \
    X2_(AW, Aruba, America, , Aruba, -4, 0)                                                        \
    X2_(PY, Asuncion, America, , Asuncion, -4, 9)                                                  \
    X2_(CA, Coral_Harbour, America, , Atikokan, -5, 0)                                                  \
    X2_(BR, Bahia, America, , Bahia, -3, 0)                                                        \
    X2_(MX, Mexico_City, America, , Bahia Banderas, -6, 8)                                      \
    X2_(BB, Barbados, America, , Barbados, -4, 0)                                                  \
    X2_(BR, Belem, America, , Belem, -3, 0)                                                        \
    X2_(BZ, Belize, America, , Belize, -6, 0)                                                      \
    X2_(CA, Blanc_Sablon, America, , Blanc - Sablon, -4, 0)                                        \
    X2_(BR, Boa_Vista, America, , Boa Vista, -4, 0)                                                \
    X2_(CO, Bogota, America, , Bogota, -5, 0)                                                      \
    X2_(US, Denver, America, , Boise, -7, 6)                                                        \
    X2_(CA, Edmonton, America, , Cambridge Bay, -7, 6)                                        \
    X2_(BR, Campo_Grande, America, , Campo Grande, -4, 20)                                         \
    X2_(MX, Cancun, America, , Cancun, -5, 8)                                                      \
    X2_(VE, Caracas, America, , Caracas, -4.5, 0)                                                  \
    X2_(GF, Cayenne, America, , Cayenne, -3, 0)                                                    \
    X2_(KY, Cayman, America, , Cayman, -5, 0)                                                      \
    X2_(US, Chicago, America, , Chicago, -6, 6)                                                    \
    X2_(MX, Chihuahua, America, , Chihuahua, -7, 8)                                                \
    X2_(CA, Coral_Harbour, America, , Coral Harbour, -5, 0)                                        \
    X2_(CR, Costa_Rica, America, , Costa Rica, -6, 0)                                              \
    X2_(CA, Creston, America, , Creston, -7, 0)                                                    \
    X2_(BR, Cuiaba, America, , Cuiaba, -4, 20)                                                     \
    X2_(CW, Curacao, America, , Curacao, -4, 0)                                                    \
    X2_(GL, Danmarkshavn, America, , Danmarkshavn, 0, 0)                                           \
    X2_(CA, Vancouver, America, , Dawson, -8, 6)                                                      \
    X2_(CA, Dawson_Creek, America, , Dawson Creek, -7, 0)                                          \
    X2_(US, Denver, America, , Denver, -7, 6)                                                      \
    X2_(US, Detroit, America, , Detroit, -5, 6)                                                    \
    X2_(DM, Dominica, America, , Dominica, -4, 0)                                                  \
    X2_(CA, Edmonton, America, , Edmonton, -7, 6)                                                  \
    X2_(BR, Rio_Branco, America, , Eirunepe, -5, 0)                                                  \
    X2_(SV, El_Salvador, America, , El Salvador, -6, 0)                                            \
    X2_(BR, Fortaleza, America, , Fortaleza, -3, 0)                                                \
    X2_(CA, Halifax, America, , Glace Bay, -4, 6)                                                \
    X2_(GL, Godthab, America, , Godthab, -3, 18)                                                   \
    X2_(CA, Goose_Bay, America, , Goose Bay, -4, 6)                                                \
    X2_(TC, Grand_Turk, America, , Grand Turk, -5, 6)                                              \
    X2_(GD, Grenada, America, , Grenada, -4, 0)                                                    \
    X2_(GP, Guadeloupe, America, , Guadeloupe, -4, 0)                                              \
    X2_(GT, Guatemala, America, , Guatemala, -6, 0)                                                \
    X2_(EC, Guayaquil, America, , Guayaquil, -5, 0)                                                \
    X2_(GY, Guyana, America, , Guyana, -4, 0)                                                      \
    X2_(CA, Halifax, America, , Halifax, -4, 6)                                                    \
    X2_(CU, Havana, America, , Havana, -5, 7)                                                      \
    X2_(MX, Hermosillo, America, , Hermosillo, -7, 0)                                              \
    X3_(US, Indianapolis, America, Indiana, Indianapolis, -5, 6)                                   \
    X3_(US, Chicago, America, Indiana, Knox, -6, 6)                                                   \
    X3_(US, Detroit, America, Indiana, Marengo, -5, 6)                                             \
    X3_(US, Detroit, America, Indiana, Petersburg, -5, 6)                                       \
    X3_(US, Chicago, America, Indiana, Tell City, -6, 6)                                         \
    X3_(US, Detroit, America, Indiana, Vevay, -5, 6)                                                 \
    X3_(US, Detroit, America, Indiana, Vincennes, -5, 6)                                         \
    X3_(US, Detroit, America, Indiana, Winamac, -5, 6)                                             \
    X2_(CA, Edmonton, America, , Inuvik, -7, 6)                                                      \
    X2_(CA, Thunder_Bay, America, , Iqaluit, -5, 6)                                                    \
    X2_(JM, Jamaica, America, , Jamaica, -5, 0)                                                    \
    X2_(US, Anchorage, America, , Juneau, -9, 6)                                                      \
    X3_(US, Louisville, America, Kentucky, Louisville, -5, 6)                                      \
    X3_(US, Detroit, America, Kentucky, Monticello, -5, 6)                                      \
    X2_(BQ, Kralendijk, America, , Kralendijk, -4, 0)                                              \
    X2_(BO, La_Paz, America, , La Paz, -4, 0)                                                      \
    X2_(PE, Lima, America, , Lima, -5, 0)                                                          \
    X2_(US, Los_Angeles, America, , Los Angeles, -8, 6)                                            \
    X2_(SX, Philipsburg, America, , Lower Princes, -4, 0)                                        \
    X2_(BR, Maceio, America, , Maceio, -3, 0)                                                      \
    X2_(NI, Managua, America, , Managua, -6, 0)                                                    \
    X2_(BR, Manaus, America, , Manaus, -4, 0)                                                      \
    X2_(MF, Marigot, America, , Marigot, -4, 0)                                                    \
    X2_(MQ, Martinique, America, , Martinique, -4, 0)                                              \
    X2_(MX, Matamoros, America, , Matamoros, -6, 6)                                                \
    X2_(MX, Mazatlan, America, , Mazatlan, -7, 8)                                                  \
    X2_(US, Chicago, America, , Menominee, -6, 6)                                                \
    X2_(MX, Guadalajara, America, , Merida, -6, 8)                                                      \
    X2_(UT, M0800, America, , Metlakatla, -8, 0)                                              \
    X2_(MX, Mexico_City, America, , Mexico City, -6, 8)                                            \
    X2_(PM, Saint_Pierre, America, , Miquelon, -3, 6)                                                  \
    X2_(CA, Goose_Bay, America, , Moncton, -4, 6)                                                    \
    X2_(MX, Monterrey, America, , Monterrey, -6, 8)                                                \
    X2_(UY, Montevideo, America, , Montevideo, -3, 6)                                              \
    X2_(CA, Montreal, America, , Montreal, -5, 6)                                                  \
    X2_(MS, Montserrat, America, , Montserrat, -4, 0)                                              \
    X2_(BS, Nassau, America, , Nassau, -5, 6)                                                      \
    X2_(US, New_York, America, , New York, -5, 6)                                                  \
    X2_(CA, Montreal, America, , Nipigon, -5, 6)                                                    \
    X2_(US, Anchorage, America, , Nome, -9, 6)                                                          \
    X2_(BR, Noronha, America, , Noronha, -2, 0)                                                    \
    X3_(US, Chicago, America, North Dakota, Beulah, -6, 6)                                          \
    X3_(US, Chicago, America, North Dakota, Center, -6, 6)                                          \
    X3_(US, Chicago, America, North Dakota, New Salem, -6, 6)                                    \
    X2_(MX, Ojinaga, America, , Ojinaga, -7, 6)                                                    \
    X2_(PA, Panama, America, , Panama, -5, 0)                                                      \
    X2_(CA, Montreal, America, , Pangnirtung, -5, 6)                                            \
    X2_(SR, Paramaribo, America, , Paramaribo, -3, 0)                                              \
    X2_(US, Phoenix, America, , Phoenix, -7, 0)                                                    \
    X2_(HT, Port_au_Prince, America, , Port - au - Prince, -5, 6)                                  \
    X2_(TT, Port_of_Spain, America, , Port of Spain, -4, 0)                                        \
    X2_(BR, Porto_Velho, America, , Porto Velho, -4, 0)                                            \
    X2_(PR, San_Juan, America, , Puerto Rico, -4, 0)                                            \
    X2_(CA, Winnipeg, America, , Rainy River, -6, 6)                                            \
    X2_(CA, Winnipeg, America, , Rankin Inlet, -6, 6)                                          \
    X2_(BR, Recife, America, , Recife, -3, 0)                                                      \
    X2_(CA, Regina, America, , Regina, -6, 0)                                                      \
    X2_(CA, Winnipeg, America, , Resolute, -6, 6)                                                  \
    X2_(BR, Rio_Branco, America, , Rio Branco, -5, 0)                                              \
    X2_(UT, P0000, America, , Santa Isabel, -8, 8)                                          \
    X2_(BR, Santarem, America, , Santarem, -3, 0)                                                  \
    X2_(CL, Santiago, America, , Santiago, -3, 0)                                                  \
    X2_(DO, Santo_Domingo, America, , Santo Domingo, -4, 0)                                        \
    X2_(BR, Sao_Paulo, America, , Sao Paulo, -3, 20)                                               \
    X2_(GL, Scoresbysund, America, , Scoresbysund, -1, 1)                                          \
    X2_(US, Salt_Lake_City, America, , Shiprock, -7, 6)                                                  \
    X2_(US, Anchorage, America, , Sitka, -9, 6)                                                        \
    X2_(BL, St_Barthelemy, America, , St Barthelemy, -4, 0)                                        \
    X2_(CA, St_Johns, America, , St Johns, -3.5, 6)                                                \
    X2_(KN, St_Kitts, America, , St Kitts, -4, 0)                                                  \
    X2_(LC, St_Lucia, America, , St Lucia, -4, 0)                                                  \
    X2_(VI, St_Thomas, America, , St Thomas, -4, 0)                                                \
    X2_(VC, St_Vincent, America, , St Vincent, -4, 0)                                              \
    X2_(CA, Regina, America, , Swift Current, -6, 0)                                        \
    X2_(HN, Tegucigalpa, America, , Tegucigalpa, -6, 0)                                            \
    X2_(GL, Thule, America, , Thule, -4, 6)                                                        \
    X2_(CA, Thunder_Bay, America, , Thunder Bay, -5, 6)                                            \
    X2_(MX, Tijuana, America, , Tijuana, -8, 6)                                                    \
    X2_(CA, Toronto, America, , Toronto, -5, 6)                                                    \
    X2_(VG, Tortola, America, , Tortola, -4, 0)                                                    \
    X2_(CA, Vancouver, America, , Vancouver, -8, 6)                                                \
    X2_(CA, Vancouver, America, , Whitehorse, -8, 6)                                              \
    X2_(CA, Winnipeg, America, , Winnipeg, -6, 6)                                                  \
    X2_(US, Anchorage, America, , Yakutat, -9, 6)                                                    \
    X2_(CA, Edmonton, America, , Yellowknife, -7, 6)                                            \
    X2_(AQ, Casey, Antarctica, , Casey, 8, 0)                                                      \
    X2_(AQ, Davis, Antarctica, , Davis, 7, 0)                                                      \
    X2_(AQ, DumontDUrville, Antarctica, , DumontDUrville, 10, 0)                                   \
    X2_(AU, Macquarie, Antarctica, , Macquarie, 11, 0)                                             \
    X2_(AQ, Mawson, Antarctica, , Mawson, 5, 0)                                                    \
    X2_(AQ, McMurdo, Antarctica, , McMurdo, 12, 255)                                               \
    X2_(AQ, Palmer, Antarctica, , Palmer, -3, 255)                                                 \
    X2_(AQ, Rothera, Antarctica, , Rothera, -3, 0)                                                 \
    X2_(AQ, South_Pole, Antarctica, , South Pole, 12, 255)                                         \
    X2_(AQ, Syowa, Antarctica, , Syowa, 3, 0)                                                      \
    X2_(UT, P0000, Antarctica, , Troll, 0, 255)                                                    \
    X2_(AQ, Vostok, Antarctica, , Vostok, 6, 0)                                                    \
    X2_(SJ, Longyearbyen, Arctic, , Longyearbyen, 1, 1)                                            \
    X2_(YE, Aden, Asia, , Aden, 3, 0)                                                              \
    X2_(KZ, Almaty, Asia, , Almaty, 6, 0)                                                          \
    X2_(JO, Amman, Asia, , Amman, 2, 19)                                                           \
    X2_(RU, Anadyr, Asia, , Anadyr, 12, 0)                                                         \
    X2_(KZ, Aqtobe, Asia, , Aqtau, 5, 0)                                                            \
    X2_(KZ, Aqtobe, Asia, , Aqtobe, 5, 0)                                                          \
    X2_(TM, Ashgabat, Asia, , Ashgabat, 5, 0)                                                      \
    X2_(IQ, Baghdad, Asia, , Baghdad, 3, 0)                                                        \
    X2_(BH, Bahrain, Asia, , Bahrain, 3, 0)                                                        \
    X2_(AZ, Baku, Asia, , Baku, 4, 3)                                                              \
    X2_(TH, Bangkok, Asia, , Bangkok, 7, 0)                                                        \
    X2_(LB, Beirut, Asia, , Beirut, 2, 4)                                                          \
    X2_(KG, Bishkek, Asia, , Bishkek, 6, 0)                                                        \
    X2_(BN, Brunei, Asia, , Brunei, 8, 0)                                                          \
    X2_(RU, Chita, Asia, , Chita, 8, 0)                                                            \
    X2_(UT, P0000, Asia, , Choibalsan, 8, 0)                                                  \
    X2_(CN, Chongqing, Asia, , Chongqing, 8, 0)                                                    \
    X2_(LK, Colombo, Asia, , Colombo, 5.5, 0)                                                      \
    X2_(SY, Damascus, Asia, , Damascus, 2, 10)                                                     \
    X2_(BD, Dhaka, Asia, , Dhaka, 6, 0)                                                            \
    X2_(TL, Dili, Asia, , Dili, 9, 0)                                                              \
    X2_(AE, Dubai, Asia, , Dubai, 4, 0)                                                            \
    X2_(TJ, Dushanbe, Asia, , Dushanbe, 5, 0)                                                      \
    X2_(PS, Gaza, Asia, , Gaza, 2, 17)                                                             \
    X2_(CN, Chongqing, Asia, , Harbin, 8, 0)                                                          \
    X2_(PS, Hebron, Asia, , Hebron, 2, 17)                                                         \
    X2_(VN, Ho_Chi_Minh, Asia, , Ho Chi Minh, 7, 0)                                                \
    X2_(HK, Hong_Kong, Asia, , Hong Kong, 8, 0)                                                    \
    X2_(MN, Hovd, Asia, , Hovd, 7, 21)                                                             \
    X2_(RU, Irkutsk, Asia, , Irkutsk, 8, 0)                                                        \
    X2_(ID, Jakarta, Asia, , Jakarta, 7, 0)                                                        \
    X2_(ID, Jayapura, Asia, , Jayapura, 9, 0)                                                      \
    X2_(IL, Jerusalem, Asia, , Jerusalem, 2, 15)                                                   \
    X2_(AF, Kabul, Asia, , Kabul, 4.5, 0)                                                          \
    X2_(RU, Kamchatka, Asia, , Kamchatka, 12, 0)                                                   \
    X2_(PK, Karachi, Asia, , Karachi, 5, 0)                                                        \
    X2_(UT, M0600, Asia, , Kashgar, 6, 0)                                                        \
    X2_(NP, Kathmandu, Asia, , Kathmandu, 5.75, 0)                                                 \
    X2_(RU, Khandyga, Asia, , Khandyga, 9, 0)                                                      \
    X2_(IN, Kolkata, Asia, , Kolkata, 5.5, 0)                                                      \
    X2_(RU, Krasnoyarsk, Asia, , Krasnoyarsk, 7, 0)                                                \
    X2_(MY, Kuala_Lumpur, Asia, , Kuala Lumpur, 8, 0)                                              \
    X2_(MY, Kuching, Asia, , Kuching, 8, 0)                                                        \
    X2_(KW, Kuwait, Asia, , Kuwait, 3, 0)                                                          \
    X2_(MO, Macau, Asia, , Macau, 8, 0)                                                            \
    X2_(RU, Magadan, Asia, , Magadan, 10, 0)                                                       \
    X2_(ID, Makassar, Asia, , Makassar, 8, 0)                                                      \
    X2_(PH, Manila, Asia, , Manila, 8, 0)                                                          \
    X2_(OM, Muscat, Asia, , Muscat, 4, 0)                                                          \
    X2_(CY, Nicosia, Asia, , Nicosia, 2, 1)                                                        \
    X2_(RU, Novosibirsk, Asia, , Novokuznetsk, 7, 0)                                              \
    X2_(RU, Novosibirsk, Asia, , Novosibirsk, 6, 0)                                                \
    X2_(RU, Omsk, Asia, , Omsk, 6, 0)                                                              \
    X2_(KZ, Aqtobe, Asia, , Oral, 5, 0)                                                              \
    X2_(KH, Phnom_Penh, Asia, , Phnom Penh, 7, 0)                                                  \
    X2_(ID, Pontianak, Asia, , Pontianak, 7, 0)                                                    \
    X2_(KP, Pyongyang, Asia, , Pyongyang, 8.5, 0)                                                  \
    X2_(QA, Qatar, Asia, , Qatar, 3, 0)                                                            \
    X2_(KZ, Almaty, Asia, , Qyzylorda, 6, 0)                                                    \
    X2_(MM, Rangoon, Asia, , Rangoon, 6.5, 0)                                                      \
    X2_(SA, Riyadh, Asia, , Riyadh, 3, 0)                                                          \
    X2_(RU, Sakhalin, Asia, , Sakhalin, 10, 0)                                                     \
    X2_(UZ, Samarkand, Asia, , Samarkand, 5, 0)                                                    \
    X2_(KR, Seoul, Asia, , Seoul, 9, 0)                                                            \
    X2_(CN, Shanghai, Asia, , Shanghai, 8, 0)                                                      \
    X2_(SG, Singapore, Asia, , Singapore, 8, 0)                                                    \
    X2_(RU, Srednekolymsk, Asia, , Srednekolymsk, 11, 0)                                           \
    X2_(TW, Taipei, Asia, , Taipei, 8, 0)                                                          \
    X2_(UZ, Tashkent, Asia, , Tashkent, 5, 0)                                                      \
    X2_(GE, Tbilisi, Asia, , Tbilisi, 4, 0)                                                        \
    X2_(IR, Tehran, Asia, , Tehran, 3.5, 16)                                                       \
    X2_(BT, Thimphu, Asia, , Thimphu, 6, 0)                                                        \
    X2_(JP, Tokyo, Asia, , Tokyo, 9, 0)                                                            \
    X2_(MN, Ulaanbaatar, Asia, , Ulaanbaatar, 8, 21)                                               \
    X2_(CN, Urumqi, Asia, , Urumqi, 6, 0)                                                          \
    X2_(RU, Ust_Nera, Asia, , Ust - Nera, 10, 0)                                                   \
    X2_(LA, Vientiane, Asia, , Vientiane, 7, 0)                                                    \
    X2_(RU, Vladivostok, Asia, , Vladivostok, 10, 0)                                               \
    X2_(RU, Yakutsk, Asia, , Yakutsk, 9, 0)                                                        \
    X2_(RU, Yekaterinburg, Asia, , Yekaterinburg, 5, 0)                                            \
    X2_(AM, Yerevan, Asia, , Yerevan, 4, 0)                                                        \
    X2_(PT, Azores, Atlantic, , Azores, -1, 1)                                                     \
    X2_(BM, Bermuda, Atlantic, , Bermuda, -4, 6)                                                   \
    X2_(ES, Canary, Atlantic, , Canary, 0, 1)                                                      \
    X2_(CV, Cape_Verde, Atlantic, , Cape Verde, -1, 0)                                             \
    X2_(FO, Faroe, Atlantic, , Faroe, 0, 1)                                                        \
    X2_(PT, Madeira, Atlantic, , Madeira, 0, 1)                                                    \
    X2_(IS, Reykjavik, Atlantic, , Reykjavik, 0, 0)                                                \
    X2_(GS, South_Georgia, Atlantic, , South Georgia, -2, 0)                                       \
    X2_(SH, St_Helena, Atlantic, , St Helena, 0, 0)                                                \
    X2_(FK, Stanley, Atlantic, , Stanley, -3, 0)                                                   \
    X2_(AU, Adelaide, Australia, , Adelaide, 9.5, 11)                                              \
    X2_(AU, Brisbane, Australia, , Brisbane, 10, 0)                                                \
    X2_(AU, Adelaide, Australia, , Broken Hill, 9.5, 11)                                        \
    X2_(AU, Sydney, Australia, , Currie, 10, 11)                                                   \
    X2_(AU, Darwin, Australia, , Darwin, 9.5, 0)                                                   \
    X2_(AU, Eucla, Australia, , Eucla, 8.75, 0)                                                    \
    X2_(AU, Hobart, Australia, , Hobart, 10, 11)                                                   \
    X2_(AU, Brisbane, Australia, , Lindeman, 10, 0)                                                \
    X2_(AU, Lord_Howe, Australia, , Lord Howe, 10.5, 12)                                           \
    X2_(AU, Melbourne, Australia, , Melbourne, 10, 11)                                             \
    X2_(AU, Perth, Australia, , Perth, 8, 0)                                                       \
    X2_(AU, Sydney, Australia, , Sydney, 10, 11)                                                   \
    X2_(NL, Amsterdam, Europe, , Amsterdam, 1, 1)                                                  \
    X2_(AD, Andorra, Europe, , Andorra, 1, 1)                                                      \
    X2_(GR, Athens, Europe, , Athens, 2, 1)                                                        \
    X2_(RS, Belgrade, Europe, , Belgrade, 1, 1)                                                    \
    X2_(DE, Berlin, Europe, , Berlin, 1, 1)                                                        \
    X2_(SK, Bratislava, Europe, , Bratislava, 1, 1)                                                \
    X2_(BE, Brussels, Europe, , Brussels, 1, 1)                                                    \
    X2_(RO, Bucharest, Europe, , Bucharest, 2, 1)                                                  \
    X2_(HU, Budapest, Europe, , Budapest, 1, 1)                                                    \
    X2_(DE, Berlin, Europe, , Busingen, 1, 1)                                                    \
    X2_(MD, Chisinau, Europe, , Chisinau, 2, 1)                                                    \
    X2_(DK, Copenhagen, Europe, , Copenhagen, 1, 1)                                                \
    X2_(IE, Dublin, Europe, , Dublin, 0, 1)                                                        \
    X2_(GI, Gibraltar, Europe, , Gibraltar, 1, 1)                                                  \
    X2_(GG, Guernsey, Europe, , Guernsey, 0, 1)                                                    \
    X2_(FI, Helsinki, Europe, , Helsinki, 2, 1)                                                    \
    X2_(IM, Isle_of_Man, Europe, , Isle of Man, 0, 1)                                              \
    X2_(TR, Istanbul, Europe, , Istanbul, 2, 1)                                                    \
    X2_(JE, Jersey, Europe, , Jersey, 0, 1)                                                        \
    X2_(RU, Kaliningrad, Europe, , Kaliningrad, 2, 0)                                              \
    X2_(UA, Kiev, Europe, , Kiev, 2, 1)                                                            \
    X2_(PT, Lisbon, Europe, , Lisbon, 0, 1)                                                        \
    X2_(SI, Ljubljana, Europe, , Ljubljana, 1, 1)                                                  \
    X2_(GB, London, Europe, , London, 0, 1)                                                        \
    X2_(LU, Luxembourg, Europe, , Luxembourg, 1, 1)                                                \
    X2_(ES, Madrid, Europe, , Madrid, 1, 1)                                                        \
    X2_(MT, Malta, Europe, , Malta, 1, 1)                                                          \
    X2_(AX, Mariehamn, Europe, , Mariehamn, 2, 1)                                                  \
    X2_(BY, Minsk, Europe, , Minsk, 3, 0)                                                          \
    X2_(MC, Monaco, Europe, , Monaco, 1, 1)                                                        \
    X2_(RU, Moscow, Europe, , Moscow, 3, 0)                                                        \
    X2_(NO, Oslo, Europe, , Oslo, 1, 1)                                                            \
    X2_(FR, Paris, Europe, , Paris, 1, 1)                                                          \
    X2_(ME, Podgorica, Europe, , Podgorica, 1, 1)                                                  \
    X2_(CZ, Prague, Europe, , Prague, 1, 1)                                                        \
    X2_(LV, Riga, Europe, , Riga, 2, 1)                                                            \
    X2_(IT, Rome, Europe, , Rome, 1, 1)                                                            \
    X2_(RU, Samara, Europe, , Samara, 4, 0)                                                        \
    X2_(SM, San_Marino, Europe, , San Marino, 1, 1)                                                \
    X2_(BA, Sarajevo, Europe, , Sarajevo, 1, 1)                                                    \
    X2_(RU, Simferopol, Europe, , Simferopol, 3, 0)                                                \
    X2_(MK, Skopje, Europe, , Skopje, 1, 1)                                                        \
    X2_(BG, Sofia, Europe, , Sofia, 2, 1)                                                          \
    X2_(SE, Stockholm, Europe, , Stockholm, 1, 1)                                                  \
    X2_(EE, Tallinn, Europe, , Tallinn, 2, 1)                                                      \
    X2_(AL, Tirane, Europe, , Tirane, 1, 1)                                                        \
    X2_(UA, Kiev, Europe, , Uzhgorod, 2, 1)                                                    \
    X2_(LI, Vaduz, Europe, , Vaduz, 1, 1)                                                          \
    X2_(VA, Vatican, Europe, , Vatican, 1, 1)                                                      \
    X2_(AT, Vienna, Europe, , Vienna, 1, 1)                                                        \
    X2_(LT, Vilnius, Europe, , Vilnius, 2, 1)                                                      \
    X2_(RU, Moscow, Europe, , Volgograd, 3, 0)                                                  \
    X2_(PL, Warsaw, Europe, , Warsaw, 1, 1)                                                        \
    X2_(HR, Zagreb, Europe, , Zagreb, 1, 1)                                                        \
    X2_(UA, Kiev, Europe, , Zaporozhye, 2, 1)                                                \
    X2_(CH, Zurich, Europe, , Zurich, 1, 1)                                                        \
    X2_(MG, Antananarivo, Indian, , Antananarivo, 3, 0)                                            \
    X2_(IO, Chagos, Indian, , Chagos, 6, 0)                                                        \
    X2_(CX, Christmas, Indian, , Christmas, 7, 0)                                                  \
    X2_(CC, Cocos, Indian, , Cocos, 6.5, 0)                                                        \
    X2_(KM, Comoro, Indian, , Comoro, 3, 0)                                                        \
    X2_(TF, Kerguelen, Indian, , Kerguelen, 5, 0)                                                  \
    X2_(SC, Mahe, Indian, , Mahe, 4, 0)                                                            \
    X2_(MV, Maldives, Indian, , Maldives, 5, 0)                                                    \
    X2_(MU, Mauritius, Indian, , Mauritius, 4, 0)                                                  \
    X2_(YT, Mayotte, Indian, , Mayotte, 3, 0)                                                      \
    X2_(RE, Reunion, Indian, , Reunion, 4, 0)                                                      \
    X2_(WS, Apia, Pacific, , Apia, 13, 13)                                                         \
    X2_(NZ, Auckland, Pacific, , Auckland, 12, 13)                                                 \
    X2_(PG, Bougainville, Pacific, , Bougainville, 11, 0)                                          \
    X2_(NZ, Chatham, Pacific, , Chatham, 12.75, 13)                                                \
    X2_(FM, Chuuk, Pacific, , Chuuk, 10, 0)                                                        \
    X2_(CL, Easter, Pacific, , Easter, -5, 0)                                                      \
    X2_(VU, Efate, Pacific, , Efate, 11, 0)                                                        \
    X2_(KI, Enderbury, Pacific, , Enderbury, 13, 0)                                                \
    X2_(TK, Fakaofo, Pacific, , Fakaofo, 13, 0)                                                    \
    X2_(FJ, Fiji, Pacific, , Fiji, 12, 14)                                                         \
    X2_(TV, Funafuti, Pacific, , Funafuti, 12, 0)                                                  \
    X2_(EC, Galapagos, Pacific, , Galapagos, -6, 0)                                                \
    X2_(PF, Gambier, Pacific, , Gambier, -9, 0)                                                    \
    X2_(SB, Guadalcanal, Pacific, , Guadalcanal, 11, 0)                                            \
    X2_(GU, Guam, Pacific, , Guam, 10, 0)                                                          \
    X2_(US, Honolulu, Pacific, , Honolulu, -10, 0)                                                 \
    X2_(UM, Johnston, Pacific, , Johnston, -10, 0)                                                 \
    X2_(KI, Kiritimati, Pacific, , Kiritimati, 14, 0)                                              \
    X2_(FM, Kosrae, Pacific, , Kosrae, 11, 0)                                                      \
    X2_(MH, Kwajalein, Pacific, , Kwajalein, 12, 0)                                                \
    X2_(MH, Majuro, Pacific, , Majuro, 12, 0)                                                      \
    X2_(PF, Marquesas, Pacific, , Marquesas, -9.5, 0)                                              \
    X2_(UM, Midway, Pacific, , Midway, -11, 0)                                                     \
    X2_(NR, Nauru, Pacific, , Nauru, 12, 0)                                                        \
    X2_(NU, Niue, Pacific, , Niue, -11, 0)                                                         \
    X2_(NF, Norfolk, Pacific, , Norfolk, 11.5, 0)                                                  \
    X2_(NC, Noumea, Pacific, , Noumea, 11, 0)                                                      \
    X2_(AS, Pago_Pago, Pacific, , Pago Pago, -11, 0)                                               \
    X2_(PW, Palau, Pacific, , Palau, 9, 0)                                                         \
    X2_(PN, Pitcairn, Pacific, , Pitcairn, -8, 0)                                                  \
    X2_(FM, Pohnpei, Pacific, , Pohnpei, 11, 0)                                                    \
    X2_(PG, Port_Moresby, Pacific, , Port Moresby, 10, 0)                                          \
    X2_(CK, Rarotonga, Pacific, , Rarotonga, -10, 0)                                               \
    X2_(MP, Saipan, Pacific, , Saipan, 10, 0)                                                      \
    X2_(PF, Tahiti, Pacific, , Tahiti, -10, 0)                                                     \
    X2_(KI, Tarawa, Pacific, , Tarawa, 12, 0)                                                      \
    X2_(TO, Tongatapu, Pacific, , Tongatapu, 13, 0)                                                \
    X2_(UM, Wake, Pacific, , Wake, 12, 0)                                                          \
    X2_(WF, Wallis, Pacific, , Wallis, 12, 0)

class TimeZone_Old
{
public:
    static const nea::TimeZone::Id TIMEZONE_MAP[];


    static TimeZone::Id convertFromOldTimeZone(uint16_t old_id, TimeZone::Id defaultValue = TimeZone::UT_P0000);
};


} // namespace nea
