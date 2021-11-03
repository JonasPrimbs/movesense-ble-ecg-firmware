#pragma once
/**
        @file TimeZone.hpp
        Timezone information interface

        @note
        Data from ambit project (timezones.sql)
*/

#include <cstdint>
#include <cstddef>

namespace nea
{

// X2_( id TEXT(2), subId, name_part_1, empty, name_part_2, offset REAL, dstrule INTEGER )
// X3_( id TEXT(2), subId, name_part_1, name_part_2, name_part_3, offset REAL, dstrule INTEGER )
// cppcheck-suppress preprocessorErrorDirective
#define FOR_TIMEZONE_X_                                                                                                          \
    X3_(DZ, Algiers, Africa, Algeria, Algiers, 1, 0)                                                                             \
    X3_(AO, Luanda, Africa, Angola, Luanda, 1, 0)                                                                                \
    X3_(BJ, Porto_Novo, Africa, Benin, Porto - Novo, 1, 0)                                                                       \
    X3_(BW, Gaborone, Africa, Botswana, Gaborone, 2, 0)                                                                          \
    X3_(BF, Ouagadougou, Africa, Burkina Faso, Ouagadougou, 0, 0)                                                                \
    X3_(BI, Bujumbura, Africa, Burundi, Bujumbura, 2, 0)                                                                         \
    X3_(CM, Douala, Africa, Cameroon, Douala, 1, 0)                                                                              \
    X3_(CF, Bangui, Africa, Central African Rep., Bangui, 1, 0)                                                                  \
    X3_(TD, Ndjamena, Africa, Chad, Ndjamena, 1, 0)                                                                              \
    X3_(KM, Comoro, Africa, Comoros, Moroni, 3, 0)                                                                               \
    X3_(CG, Brazzaville, Africa, Congo, Brazzaville, 1, 0)                                                                       \
    X3_(CD, Kinshasa, Africa, Congo Dem.Rep., Kinshasa, 1, 0)                                                                    \
    X3_(CD, Lubumbashi, Africa, Congo Dem.Rep., Lubumbashi, 2, 0)                                                                \
    X3_(DJ, Djibouti, Africa, Djibouti, Djibouti, 3, 0)                                                                          \
    X3_(ER, Asmara, Africa, Eritrea, Asmara, 3, 0)                                                                               \
    X3_(EG, Cairo, Africa, Egypt, Cairo, 2, 0)                                                                                   \
    X3_(GQ, Malabo, Africa, Equatorial Guinea, Malabo, 1, 0)                                                                     \
    X3_(ET, Addis_Ababa, Africa, Ethiopia, Addis Ababa, 3, 0)                                                                    \
    X3_(GA, Libreville, Africa, Gabon, Libreville, 1, 0)                                                                         \
    X3_(GM, Banjul, Africa, Gambia, Banjul, 0, 0)                                                                                \
    X3_(GH, Accra, Africa, Ghana, Accra, 0, 0)                                                                                   \
    X3_(GN, Conakry, Africa, Guinea, Conakry, 0, 0)                                                                              \
    X3_(GW, Bissau, Africa, Guinea - Bissau, Bissau, 0, 0)                                                                       \
    X3_(CI, Abidjan, Africa, Ivory Coast, Abidjan, 0, 0)                                                                         \
    X3_(KE, Nairobi, Africa, Kenya, Nairobi, 3, 0)                                                                               \
    X3_(LS, Maseru, Africa, Lesotho, Maseru, 2, 0)                                                                               \
    X3_(LR, Monrovia, Africa, Liberia, Monrovia, 0, 0)                                                                           \
    X3_(LY, Tripoli, Africa, Libya, Tripoli, 2, 0)                                                                               \
    X3_(MG, Antananarivo, Africa, Madagascar, Antananarivo, 3, 0)                                                                \
    X3_(MW, Blantyre, Africa, Malawi, Blantyre, 2, 0)                                                                            \
    X3_(ML, Bamako, Africa, Mali, Bamako, 0, 0)                                                                                  \
    X3_(MR, Nouakchott, Africa, Mauritania, Nouakchott, 0, 0)                                                                    \
    X3_(YT, Mayotte, Africa, Mayotte, Mamoutzou, 3, 0)                                                                           \
    X3_(MA, Casablanca, Africa, Morocco, Casablanca, 0, 2)                                                                       \
    X3_(MZ, Maputo, Africa, Mozambique, Maputo, 2, 0)                                                                            \
    X3_(NA, Windhoek, Africa, Namibia, Windhoek, 1, 5)                                                                           \
    X3_(NE, Niamey, Africa, Niger, Niamey, 1, 0)                                                                                 \
    X3_(NG, Lagos, Africa, Nigeria, Lagos, 1, 0)                                                                                 \
    X3_(RW, Kigali, Africa, Rwanda, Kigali, 2, 0)                                                                                \
    X3_(ST, Sao_Tome, Africa, São Tomé and Pr., São Tomé, 0, 0)                                                                  \
    X3_(SN, Dakar, Africa, Senegal, Dakar, 0, 0)                                                                                 \
    X3_(SL, Freetown, Africa, Sierra Leone, Freetown, 0, 0)                                                                      \
    X3_(ES, Ceuta, Africa, Spain, Ceuta, 1, 1)                                                                                   \
    X3_(SO, Mogadishu, Africa, Somalia, Mogadishu, 3, 0)                                                                         \
    X3_(ZA, Johannesburg, Africa, South Africa, Johannesburg, 2, 0)                                                              \
    X3_(SS, Juba, Africa, South Sudan, Juba, 3, 0)                                                                               \
    X3_(SD, Khartoum, Africa, Sudan, Khartoum, 3, 0)                                                                             \
    X3_(SZ, Mbabane, Africa, Swaziland, Mbabane, 2, 0)                                                                           \
    X3_(TZ, Dar_es_Salaam, Africa, Tanzania, Dar es Salaam, 3, 0)                                                                \
    X3_(TG, Lome, Africa, Togo, Lome, 0, 0)                                                                                      \
    X3_(TN, Tunis, Africa, Tunisia, Tunis, 1, 0)                                                                                 \
    X3_(UG, Kampala, Africa, Uganda, Kampala, 3, 0)                                                                              \
    X3_(EH, El_Aaiun, Africa, Western Sahara, El Aaiun, 0, 2)                                                                    \
    X3_(ZM, Lusaka, Africa, Zambia, Lusaka, 2, 0)                                                                                \
    X3_(ZW, Harare, Africa, Zimbabwe, Harare, 2, 0)                                                                              \
    X3_(AI, Anguilla, America, Anguilla, The Valley, -4, 0)                                                                      \
    X3_(AG, Antigua, America, Antigua and Barbuda, Saint Johns, -4, 0)                                                           \
    X3_(AR, Buenos_Aires, America, Argentina, Buenos Aires, -3, 0)                                                               \
    X3_(AR, Cordoba, America, Argentina, Córdoba, -3, 0)                                                                         \
    X3_(AR, Mendoza, America, Argentina, Mendoza, -3, 0)                                                                         \
    X3_(AR, Rio_Gallegos, America, Argentina, Rio Gallegos, -3, 0)                                                               \
    X3_(AR, Salta, America, Argentina, Salta, -3, 0)                                                                             \
    X3_(AR, Ushuaia, America, Argentina, Ushuaia, -3, 0)                                                                         \
    X3_(BS, Nassau, America, Bahamas, Nassau, -5, 6)                                                                             \
    X3_(BB, Barbados, America, Barbados, Bridgetown, -4, 0)                                                                      \
    X3_(BZ, Belize, America, Belize, Belmopan, -6, 0)                                                                            \
    X3_(BO, La_Paz, America, Bolivia, La Paz, -4, 0)                                                                             \
    X3_(BQ, Kralendijk, America, Bonaire, Kralendijk, -4, 0)                                                                     \
    X3_(BR, Araguaina, America, Brazil, Araguaína, -3, 0)                                                                        \
    X3_(BR, Belem, America, Brazil, Belém, -3, 0)                                                                                \
    X3_(BR, Boa_Vista, America, Brazil, Boa Vista, -4, 0)                                                                        \
    X3_(BR, Campo_Grande, America, Brazil, Campo Grande, -4, 20)                                                                 \
    X3_(BR, Cuiaba, America, Brazil, Cuiabá, -4, 20)                                                                             \
    X3_(BR, Fortaleza, America, Brazil, Fortaleza, -3, 0)                                                                        \
    X3_(BR, Noronha, America, Brazil, Fernando de Noronha, -2, 0)                                                                \
    X3_(BR, Maceio, America, Brazil, Maceió, -3, 0)                                                                              \
    X3_(BR, Manaus, America, Brazil, Manaus, -4, 0)                                                                              \
    X3_(BR, Porto_Velho, America, Brazil, Porto Velho, -4, 0)                                                                    \
    X3_(BR, Recife, America, Brazil, Recife, -3, 0)                                                                              \
    X3_(BR, Rio_Branco, America, Brazil, Rio Branco, -5, 0)                                                                      \
    X3_(BR, Rio_de_Janeiro, America, Brazil, Rio de Janeiro, -3, 20)                                                             \
    X3_(BR, Bahia, America, Brazil, Salvador, -3, 0)                                                                             \
    X3_(BR, Santarem, America, Brazil, Santarém, -3, 0)                                                                          \
    X3_(BR, Sao_Paulo, America, Brazil, São Paulo, -3, 20)                                                                       \
    X3_(VG, Tortola, America, British Virgin Isl., Road Town, -4, 0)                                                             \
    X3_(CA, Blanc_Sablon, America, Canada, Blanc - Sablon, -4, 0)                                                                \
    X3_(CA, Creston, America, Canada, Creston, -7, 0)                                                                            \
    X3_(CA, Coral_Harbour, America, Canada, Coral Harbour, -5, 0)                                                                \
    X3_(CA, Edmonton, America, Canada, Edmonton, -7, 6)                                                                          \
    X3_(CA, Dawson_Creek, America, Canada, Fort St John, -7, 0)                                                                  \
    X3_(CA, Goose_Bay, America, Canada, Goose Bay, -4, 6)                                                                        \
    X3_(CA, Halifax, America, Canada, Halifax, -4, 6)                                                                            \
    X3_(CA, Montreal, America, Canada, Montreal, -5, 6)                                                                          \
    X3_(CA, Regina, America, Canada, Regina, -6, 0)                                                                              \
    X3_(CA, St_Johns, America, Canada, St Johns, -3.5, 6)                                                                        \
    X3_(CA, Thunder_Bay, America, Canada, Thunder Bay, -5, 6)                                                                    \
    X3_(CA, Toronto, America, Canada, Toronto, -5, 6)                                                                            \
    X3_(CA, Vancouver, America, Canada, Vancouver, -8, 6)                                                                        \
    X3_(CA, Winnipeg, America, Canada, Winnipeg, -6, 6)                                                                          \
    X3_(KY, Cayman, America, Cayman Islands, George Town, -5, 0)                                                                 \
    X3_(CL, Santiago, America, Chile, Santiago, -3, 0) /* MISSING DST RULE */                                                    \
    X3_(CO, Bogota, America, Colombia, Bogota, -5, 0)                                                                            \
    X3_(CR, Costa_Rica, America, Costa Rica, San José, -6, 0)                                                                    \
    X3_(CU, Havana, America, Cuba, Havana, -5, 7)                                                                                \
    X3_(CW, Curacao, America, Curaçao, Willemstad, -4, 0)                                                                        \
    X3_(DM, Dominica, America, Dominica, Roseau, -4, 0)                                                                          \
    X3_(DO, Santo_Domingo, America, Dominican Republic, Santo Domingo, -4, 0)                                                    \
    X3_(EC, Guayaquil, America, Ecuador, Guayaquil, -5, 0)                                                                       \
    X3_(SV, El_Salvador, America, El Salvador, San Salvador, -6, 0)                                                              \
    X3_(GF, Cayenne, America, French Guiana, Cayenne, -3, 0)                                                                     \
    X3_(GP, Guadeloupe, America, Guadeloupe, Basse - Terre, -4, 0)                                                               \
    X3_(GT, Guatemala, America, Guatemala, Guatemala, -6, 0)                                                                     \
    X3_(GY, Guyana, America, Guyana, Georgetown, -4, 0)                                                                          \
    X3_(HT, Port_au_Prince, America, Haiti, Port - au - Prince, -5, 6)                                                           \
    X3_(HN, Tegucigalpa, America, Honduras, Tegucigalpa, -6, 0)                                                                  \
    X3_(JM, Jamaica, America, Jamaica, Kingston, -5, 0)                                                                          \
    X3_(MQ, Martinique, America, Martinique, Fort - de - France, -4, 0)                                                          \
    X3_(MX, Guadalajara, America, Mexico, Guadalajara, -6, 8)                                                                    \
    X3_(MX, Cancun, America, Mexico, Cancún, -5, 0)                                                                              \
    X3_(MX, Chihuahua, America, Mexico, Chihuahua, -7, 8)                                                                        \
    X3_(MX, Hermosillo, America, Mexico, Hermosillo, -7, 0)                                                                      \
    X3_(MX, Matamoros, America, Mexico, Matamoros, -6, 6)                                                                        \
    X3_(MX, Mazatlan, America, Mexico, Mazatlan, -7, 8)                                                                          \
    X3_(MX, Mexico_City, America, Mexico, Mexico City, -6, 8)                                                                    \
    X3_(MX, Monterrey, America, Mexico, Monterrey, -6, 8)                                                                        \
    X3_(MX, Ojinaga, America, Mexico, Ciudad Juarez, -7, 6)                                                                      \
    X3_(MX, Tijuana, America, Mexico, Tijuana, -8, 6)                                                                            \
    X3_(MS, Montserrat, America, Montserrat, Brades, -4, 0)                                                                      \
    X3_(AW, Aruba, America, Netherlands, Aruba, -4, 0)                                                                           \
    X3_(NI, Managua, America, Nicaragua, Managua, -6, 0)                                                                         \
    X3_(AK, Alaska_Time, America, North American, Alaska Time, -9, 6)                                                            \
    X3_(CT, Central_Time, America, North American, Central Time, -6, 6)                                                          \
    X3_(ET, Eastern_Time, America, North American, Eastern Time, -5, 6)                                                          \
    X3_(MT, Mountain_Time, America, North American, Mountain Time, -7, 6)                                                        \
    X3_(PT, Pacific_Time, America, North American, Pacific Time, -8, 6)                                                          \
    X3_(PA, Panama, America, Panama, Panama, -5, 0)                                                                              \
    X3_(PY, Asuncion, America, Paraguay, Asuncion, -4, 9)                                                                        \
    X3_(PE, Lima, America, Peru, Lima, -5, 0)                                                                                    \
    X3_(PR, San_Juan, America, Puerto Rico, San Juan, -4, 0)                                                                     \
    X3_(BL, St_Barthelemy, America, St Barthélemy, Gustavia, -4, 0)                                                              \
    X3_(GD, Grenada, America, St Georges, Grenada, -4, 0)                                                                        \
    X3_(KN, St_Kitts, America, St Kitts and N., Basseterre, -4, 0)                                                               \
    X3_(LC, St_Lucia, America, St Lucia, Castries, -4, 0)                                                                        \
    X3_(SX, Philipsburg, America, St Maarten, Philipsburg, -4, 0)                                                                \
    X3_(MF, Marigot, America, St Martin, Marigot, -4, 0)                                                                         \
    X3_(PM, Saint_Pierre, America, St Pierre and Miq., Saint - Pierre, -3, 6)                                                    \
    X3_(VC, St_Vincent, America, St Vincent and Gr., Kingstown, -4, 0)                                                           \
    X3_(SR, Paramaribo, America, Suriname, Paramaribo, -3, 0)                                                                    \
    X3_(TT, Port_of_Spain, America, Trinidad and Tobago, Port of Spain, -4, 0)                                                   \
    X3_(TC, Grand_Turk, America, Turks and Caicos Isl., Cockburn Town, -4, 0)                                                    \
    X3_(UY, Montevideo, America, Uruguay, Montevideo, -3, 0)                                                                     \
    X3_(US, Adak, America, USA, Adak, -10, 6)                                                                                    \
    X3_(US, Anchorage, America, USA, Anchorage, -9, 6)                                                                           \
    X3_(US, Chicago, America, USA, Chicago, -6, 6)                                                                               \
    X3_(US, Denver, America, USA, Denver, -7, 6)                                                                                 \
    X3_(US, Detroit, America, USA, Detroit, -5, 6)                                                                               \
    X3_(US, Indianapolis, America, USA, Indianapolis, -5, 6)                                                                     \
    X3_(US, Honolulu, America, USA, Honolulu, -10, 0)                                                                            \
    X3_(CT, Houston, America, USA, Houston, -6, 6)                                                                               \
    X3_(US, Los_Angeles, America, USA, Los Angeles, -8, 6)                                                                       \
    X3_(US, Louisville, America, USA, Louisville, -5, 6)                                                                         \
    X3_(US, New_York, America, USA, New York, -5, 6)                                                                             \
    X3_(US, Philadelphia, America, USA, Philadelphia, -5, 6)                                                                     \
    X3_(US, Phoenix, America, USA, Phoenix, -7, 0)                                                                               \
    X3_(US, Salt_Lake_City, America, USA, Salt Lake City, -7, 6)                                                                 \
    X3_(VI, St_Thomas, America, US Virgin Isl., Charlotte Amalie, -4, 0)                                                         \
    X3_(VE, Caracas, America, Venezuela, Caracas, -4, 0)                                                                         \
    X3_(AQ, Casey, Antarctica, Antarctica, Casey, 8, 0)                                                                          \
    X3_(AQ, Davis, Antarctica, Antarctica, Davis, 7, 0)                                                                          \
    X3_(AQ, DumontDUrville, Antarctica, Antarctica, Dumont d\x27Urville, 10, 0)                                                  \
    X3_(AU, Macquarie, Antarctica, Antarctica, Macquarie, 11, 0)                                                                 \
    X3_(AQ, Mawson, Antarctica, Antarctica, Mawson, 5, 0)                                                                        \
    X3_(AQ, McMurdo, Antarctica, Antarctica, McMurdo, 12, 255)                                                                   \
    X3_(AQ, Palmer, Antarctica, Antarctica, Palmer, -3, 255)                                                                     \
    X3_(AQ, Rothera, Antarctica, Antarctica, Rothera, -3, 0)                                                                     \
    X3_(AQ, South_Pole, Antarctica, Antarctica, South Pole, 12, 255)                                                             \
    X3_(AQ, Syowa, Antarctica, Antarctica, Syowa, 3, 0)                                                                          \
    X3_(AQ, Vostok, Antarctica, Antarctica, Vostok, 6, 0)                                                                        \
    X3_(AZ, Baku, Asia, Azerbaijan, Baku, 4, 0)                                                                                  \
    X3_(AF, Kabul, Asia, Afghanistan, Kabul, 4.5, 0)                                                                             \
    X3_(AM, Yerevan, Asia, Armenia, Yerevan, 4, 0)                                                                               \
    X3_(BH, Bahrain, Asia, Bahrain, Manama, 3, 0)                                                                                \
    X3_(BD, Dhaka, Asia, Bangladesh, Dhaka, 6, 0)                                                                                \
    X3_(BT, Thimphu, Asia, Bhutan, Thimphu, 6, 0)                                                                                \
    X3_(BN, Brunei, Asia, Brunei, Bandar Seri B., 8, 0)                                                                          \
    X3_(KH, Phnom_Penh, Asia, Cambodia, Phnom Penh, 7, 0)                                                                        \
    X3_(CN, Beijing, Asia, China, Beijing, 8, 0)                                                                                 \
    X3_(CN, Chongqing, Asia, China, Chongqing, 8, 0)                                                                             \
    X3_(CN, Lhasa, Asia, China, Lhasa, 8, 0)                                                                                     \
    X3_(CN, Shanghai, Asia, China, Shanghai, 8, 0)                                                                               \
    X3_(CN, Shenzhen, Asia, China, Shenzhen, 8, 0)                                                                               \
    X3_(CN, Urumqi, Asia, China, Ürümqi, 8, 0)                                                                                   \
    X3_(GE, Tbilisi, Asia, Georgia, Tbilisi, 4, 0)                                                                               \
    X3_(HK, Hong_Kong, Asia, Hong Kong, Hong Kong, 8, 0)                                                                         \
    X3_(IN, Kolkata, Asia, India, Kolkata, 5.5, 0)                                                                               \
    X3_(ID, Jakarta, Asia, Indonesia, Jakarta, 7, 0)                                                                             \
    X3_(ID, Jayapura, Asia, Indonesia, Jayapura, 9, 0)                                                                           \
    X3_(ID, Makassar, Asia, Indonesia, Makassar, 8, 0)                                                                           \
    X3_(ID, Pontianak, Asia, Indonesia, Pontianak, 7, 0)                                                                         \
    X3_(IQ, Baghdad, Asia, Iraq, Baghdad, 3, 0)                                                                                  \
    X3_(IR, Tehran, Asia, Iran, Tehran, 3.5, 16)                                                                                 \
    X3_(IL, Jerusalem, Asia, Israel, Jerusalem, 2, 15)                                                                           \
    X3_(JP, Tokyo, Asia, Japan, Tokyo, 9, 0)                                                                                     \
    X3_(JO, Amman, Asia, Jordan, Amman, 2, 19)                                                                                   \
    X3_(KZ, Almaty, Asia, Kazakhstan, Almaty, 6, 0)                                                                              \
    X3_(KZ, Aqtobe, Asia, Kazakhstan, Aqtobe, 5, 0)                                                                              \
    X3_(KW, Kuwait, Asia, Kuwait, Kuwait City, 3, 0)                                                                             \
    X3_(KG, Bishkek, Asia, Kyrgyzstan, Bishkek, 6, 0)                                                                            \
    X3_(LA, Vientiane, Asia, Laos, Vientiane, 7, 0)                                                                              \
    X3_(LB, Beirut, Asia, Lebanon, Beirut, 2, 4)                                                                                 \
    X3_(MY, Kuala_Lumpur, Asia, Malaysia, Kuala Lumpur, 8, 0)                                                                    \
    X3_(MY, Kuching, Asia, Malaysia, Kuching, 8, 0)                                                                              \
    X3_(MO, Macau, Asia, Macao, Macau, 8, 0)                                                                                     \
    X3_(MN, Hovd, Asia, Mongolia, Hovd, 7, 0)                                                                                    \
    X3_(MN, Ulaanbaatar, Asia, Mongolia, Ulaanbaatar, 8, 0)                                                                      \
    X3_(MM, Rangoon, Asia, Myanmar, Rangoon, 6.5, 0)                                                                             \
    X3_(NP, Kathmandu, Asia, Nepal, Kathmandu, 5.75, 0)                                                                          \
    X3_(KP, Pyongyang, Asia, North Korea, Pyongyang, 8.5, 0)                                                                     \
    X3_(OM, Muscat, Asia, Oman, Muscat, 4, 0)                                                                                    \
    X3_(PK, Karachi, Asia, Pakistan, Karachi, 5, 0)                                                                              \
    X3_(PS, Gaza, Asia, Palestine, Gaza, 2, 17)                                                                                  \
    X3_(PS, Hebron, Asia, Palestine, Hebron, 2, 17)                                                                              \
    X3_(PH, Manila, Asia, Philippines, Manila, 8, 0)                                                                             \
    X3_(LK, Colombo, Asia, Sri Lanka, Colombo, 5.5, 0)                                                                           \
    X3_(SY, Damascus, Asia, Syria, Damascus, 2, 10)                                                                              \
    X3_(QA, Qatar, Asia, Qatar, Doha, 3, 0)                                                                                      \
    X3_(RU, Anadyr, Asia, Russia, Anadyr, 12, 0)                                                                                 \
    X3_(RU, Chita, Asia, Russia, Chita, 9, 0)                                                                                    \
    X3_(RU, Irkutsk, Asia, Russia, Irkutsk, 8, 0)                                                                                \
    X3_(RU, Kamchatka, Asia, Russia, Kamchatka, 12, 0)                                                                           \
    X3_(RU, Khandyga, Asia, Russia, Khandyga, 9, 0)                                                                              \
    X3_(RU, Krasnoyarsk, Asia, Russia, Krasnoyarsk, 7, 0)                                                                        \
    X3_(RU, Magadan, Asia, Russia, Magadan, 11, 0)                                                                               \
    X3_(RU, Novosibirsk, Asia, Russia, Novosibirsk, 7, 0)                                                                        \
    X3_(RU, Omsk, Asia, Russia, Omsk, 6, 0)                                                                                      \
    X3_(RU, Sakhalin, Asia, Russia, Sakhalin, 11, 0)                                                                             \
    X3_(RU, Srednekolymsk, Asia, Russia, Srednekolymsk, 11, 0)                                                                   \
    X3_(RU, Ust_Nera, Asia, Russia, Ust - Nera, 10, 0)                                                                           \
    X3_(RU, Yekaterinburg, Asia, Russia, Yekaterinburg, 5, 0)                                                                    \
    X3_(RU, Vladivostok, Asia, Russia, Vladivostok, 10, 0)                                                                       \
    X3_(RU, Yakutsk, Asia, Russia, Yakutsk, 9, 0)                                                                                \
    X3_(SA, Riyadh, Asia, Saudi Arabia, Riyadh, 3, 0)                                                                            \
    X3_(SG, Singapore, Asia, Singapore, Singapore, 8, 0)                                                                         \
    X3_(KR, Seoul, Asia, South Korea, Seoul, 9, 0)                                                                               \
    X3_(TW, Taipei, Asia, Taiwan, Taipei, 8, 0)                                                                                  \
    X3_(TJ, Dushanbe, Asia, Tajikistan, Dushanbe, 5, 0)                                                                          \
    X3_(TH, Bangkok, Asia, Thailand, Bangkok, 7, 0)                                                                              \
    X3_(TL, Dili, Asia, Timor - Leste, Dili, 9, 0)                                                                               \
    X3_(TM, Ashgabat, Asia, Turkmenistan, Ashgabat, 5, 0)                                                                        \
    X3_(AE, Dubai, Asia, United Arab Emir., Dubai, 4, 0)                                                                         \
    X3_(UZ, Samarkand, Asia, Uzbekistan, Samarkand, 5, 0)                                                                        \
    X3_(UZ, Tashkent, Asia, Uzbekistan, Tashkent, 5, 0)                                                                          \
    X3_(VN, Ho_Chi_Minh, Asia, Vietnam, Ho Chi Minh, 7, 0)                                                                       \
    X3_(YE, Aden, Asia, Yemen, Aden, 3, 0)                                                                                       \
    X3_(PT, Azores, Atlantic, Azores, Ponta Delgada, -1, 1)                                                                      \
    X3_(BM, Bermuda, Atlantic, Bermuda, Hamilton, -4, 6)                                                                         \
    X3_(CV, Cape_Verde, Atlantic, Cape Verde, Praia, -1, 0)                                                                      \
    X3_(FK, Stanley, Atlantic, Falkland Isl., Stanley, -3, 0)                                                                    \
    X3_(PT, Madeira, Atlantic, Portugal, Madeira, 0, 1)                                                                          \
    X3_(GS, South_Georgia, Atlantic, South Georgia, King Edw.P., -2, 0)                                                          \
    X3_(ES, Canary, Atlantic, Spain, Canary Islands, 0, 1)                                                                       \
    X3_(SH, St_Helena, Atlantic, St Helena, Jamestown, 0, 0)                                                                     \
    X3_(AU, Adelaide, Australia, Australia, Adelaide, 9.5, 11)                                                                   \
    X3_(AU, Brisbane, Australia, Australia, Brisbane, 10, 0)                                                                     \
    X3_(AU, Darwin, Australia, Australia, Darwin, 9.5, 0)                                                                        \
    X3_(AU, Eucla, Australia, Australia, Eucla, 8.75, 0)                                                                         \
    X3_(AU, Hobart, Australia, Australia, Hobart, 10, 11)                                                                        \
    X3_(AU, Lord_Howe, Australia, Australia, Lord Howe, 10.5, 12)                                                                \
    X3_(AU, Melbourne, Australia, Australia, Melbourne, 10, 11)                                                                  \
    X3_(AU, Perth, Australia, Australia, Perth, 8, 0)                                                                            \
    X3_(AU, Sydney, Australia, Australia, Sydney, 10, 11)                                                                        \
    X3_(AS, Pago_Pago, Australia, American Samoa, Pago Pago, -11, 0)                                                             \
    X3_(CL, Easter, Australia, Chile, Easter Island, -5, 0) /* MISSING DST RULE */                                               \
    X3_(CX, Christmas, Australia, Christmas Island, The Settlement, 7, 0)                                                        \
    X3_(CK, Rarotonga, Australia, Cook Islands, Rarotonga, -10, 0)                                                               \
    X3_(EC, Galapagos, Australia, Ecuador, Galapagos, -6, 0)                                                                     \
    X3_(FJ, Fiji, Australia, Fiji, Nadi, 12, 14)                                                                                 \
    X3_(PF, Marquesas, Australia, French Polynesia, Marquesas, -9.5, 0)                                                          \
    X3_(PF, Gambier, Australia, French Polynesia, Gambier, -9, 0)                                                                \
    X3_(PF, Tahiti, Australia, French Polynesia, Tahiti, -10, 0)                                                                 \
    X3_(GU, Guam, Australia, Guam, Hagåtña, 10, 0)                                                                               \
    X3_(KI, Enderbury, Australia, Kiribati, Kanton, 13, 0)                                                                       \
    X3_(KI, Kiritimati, Australia, Kiribati, Kiritimati, 14, 0)                                                                  \
    X3_(KI, Tarawa, Australia, Kiribati, Tarawa, 12, 0)                                                                          \
    X3_(MH, Kwajalein, Australia, Marshall Islands, Kwajalein, 12, 0)                                                            \
    X3_(MH, Majuro, Australia, Marshall Islands, Majuro, 12, 0)                                                                  \
    X3_(FM, Chuuk, Australia, Micronesia, Chuuk, 10, 0)                                                                          \
    X3_(FM, Kosrae, Australia, Micronesia, Kosrae, 11, 0)                                                                        \
    X3_(FM, Pohnpei, Australia, Micronesia, Pohnpei, 11, 0)                                                                      \
    X3_(NR, Nauru, Australia, Nauru, Yaren, 12, 0)                                                                               \
    X3_(NC, Noumea, Australia, New Caledonia, Noumea, 11, 0)                                                                     \
    X3_(NZ, Auckland, Australia, New Zealand, Auckland, 12, 13)                                                                  \
    X3_(NZ, Chatham, Australia, New Zealand, Chatham Islands, 12.75, 13)                                                         \
    X3_(NU, Niue, Australia, Niue, Alofi, -11, 0)                                                                                \
    X3_(NF, Norfolk, Australia, Norfolk Island, Kingston, 11, 0)                                                                 \
    X3_(MP, Saipan, Australia, Northern Mariana Isl., Saipan, 10, 0)                                                             \
    X3_(PW, Palau, Australia, Palau, Koror, 9, 0)                                                                                \
    X3_(PG, Bougainville, Australia, Papua New Guinea, Bougainville, 11, 0)                                                      \
    X3_(PG, Port_Moresby, Australia, Papua New Guinea, Port Moresby, 10, 0)                                                      \
    X3_(PN, Pitcairn, Australia, Pitcairn, Adamstown, -8, 0)                                                                     \
    X3_(WS, Apia, Australia, Samoa, Apia, 13, 13)                                                                                \
    X3_(SB, Guadalcanal, Australia, Solomon Islands, Honiara, 11, 0)                                                             \
    X3_(TK, Fakaofo, Australia, Tokelau, Fakaofo, 13, 0)                                                                         \
    X3_(TO, Tongatapu, Australia, Tonga, Nukualofa, 13, 14)                                                                      \
    X3_(TV, Funafuti, Australia, Tuvalu, Funafuti, 12, 0)                                                                        \
    X3_(UM, Midway, Australia, USA, Midway Atoll, -11, 0)                                                                        \
    X3_(UM, Johnston, Australia, USA, Johnston Atoll, -10, 0)                                                                    \
    X3_(UM, Wake, Australia, USA, Wake Island, 12, 0)                                                                            \
    X3_(VU, Efate, Australia, Vanuatu, Port Vila, 11, 0)                                                                         \
    X3_(WF, Wallis, Australia, Wallis, Mata - Utu, 12, 0)                                                                        \
    X3_(AL, Tirane, Europe, Albania, Tirane, 1, 1)                                                                               \
    X3_(AD, Andorra, Europe, Andorra, Andorra, 1, 1)                                                                             \
    X3_(AT, Vienna, Europe, Austria, Vienna, 1, 1)                                                                               \
    X3_(BY, Minsk, Europe, Belarus, Minsk, 3, 0)                                                                                 \
    X3_(BE, Brussels, Europe, Belgium, Brussels, 1, 1)                                                                           \
    X3_(BA, Sarajevo, Europe, Bosnia - Herzegovina, Sarajevo, 1, 1)                                                              \
    X3_(BG, Sofia, Europe, Bulgaria, Sofia, 2, 1)                                                                                \
    X3_(HR, Zagreb, Europe, Croatia, Zagreb, 1, 1)                                                                               \
    X3_(CZ, Prague, Europe, Czech Republic, Prague, 1, 1)                                                                        \
    X3_(CY, Kyrenia, Europe, Cyprus, Kyrenia, 3, 0)                                                                              \
    X3_(CY, Nicosia, Europe, Cyprus, Nicosia, 2, 1)                                                                              \
    X3_(DK, Copenhagen, Europe, Denmark, Copenhagen, 1, 1)                                                                       \
    X3_(EE, Tallinn, Europe, Estonia, Tallinn, 2, 1)                                                                             \
    X3_(FO, Faroe, Europe, Faroe Islands, Tórshavn, 0, 1)                                                                        \
    X3_(FI, Helsinki, Europe, Finland, Helsinki, 2, 1)                                                                           \
    X3_(FR, Paris, Europe, France, Paris, 1, 1)                                                                                  \
    X3_(DE, Berlin, Europe, Germany, Berlin, 1, 1)                                                                               \
    X3_(GI, Gibraltar, Europe, Gibraltar, Gibraltar, 1, 1)                                                                       \
    X3_(GR, Athens, Europe, Greece, Athens, 2, 1)                                                                                \
    X3_(GL, Danmarkshavn, Europe, Greenland, Danmarkshavn, 0, 0)                                                                 \
    X3_(GL, Godthab, Europe, Greenland, Nuuk, -3, 18)                                                                            \
    X3_(GL, Scoresbysund, Europe, Greenland, Scoresbysund, -1, 1)                                                                \
    X3_(GL, Thule, Europe, Greenland, Thule Air Base, -4, 6)                                                                     \
    X3_(GG, Guernsey, Europe, Guernsey, St.Peter Port, 0, 1)                                                                     \
    X3_(HU, Budapest, Europe, Hungary, Budapest, 1, 1)                                                                           \
    X3_(IS, Reykjavik, Europe, Iceland, Reykjavik, 0, 0)                                                                         \
    X3_(IE, Dublin, Europe, Ireland, Dublin, 0, 1)                                                                               \
    X3_(IM, Isle_of_Man, Europe, Isle of Man, Douglas, 0, 1)                                                                     \
    X3_(IT, Rome, Europe, Italy, Rome, 1, 1)                                                                                     \
    X3_(JE, Jersey, Europe, Jersey, Saint Helier, 0, 1)                                                                          \
    X3_(LV, Riga, Europe, Latvia, Riga, 2, 1)                                                                                    \
    X3_(LI, Vaduz, Europe, Liechtenstein, Vaduz, 1, 1)                                                                           \
    X3_(LT, Vilnius, Europe, Lithuania, Vilnius, 2, 1)                                                                           \
    X3_(LU, Luxembourg, Europe, Luxembourg, Luxembourg, 1, 1)                                                                    \
    X3_(MK, Skopje, Europe, Macedonia, Skopje, 1, 1)                                                                             \
    X3_(MT, Malta, Europe, Malta, Valletta, 1, 1)                                                                                \
    X3_(MD, Chisinau, Europe, Moldova, Chișinău, 2, 1)                                                                           \
    X3_(MC, Monaco, Europe, Monaco, Monaco, 1, 1)                                                                                \
    X3_(ME, Podgorica, Europe, Montenegro, Podgorica, 1, 1)                                                                      \
    X3_(NL, Amsterdam, Europe, Netherlands, Amsterdam, 1, 1)                                                                     \
    X3_(NO, Oslo, Europe, Norway, Oslo, 1, 1)                                                                                    \
    X3_(PL, Warsaw, Europe, Poland, Warsaw, 1, 1)                                                                                \
    X3_(PT, Lisbon, Europe, Portugal, Lisbon, 0, 1)                                                                              \
    X3_(RO, Bucharest, Europe, Romania, Bucharest, 2, 1)                                                                         \
    X3_(RU, Kaliningrad, Europe, Russia, Kaliningrad, 2, 0)                                                                      \
    X3_(RU, Moscow, Europe, Russia, Moscow, 3, 0)                                                                                \
    X3_(RU, Samara, Europe, Russia, Samara, 4, 0)                                                                                \
    X3_(SM, San_Marino, Europe, San Marino, San Marino, 1, 1)                                                                    \
    X3_(RS, Belgrade, Europe, Serbia, Belgrade, 1, 1)                                                                            \
    X3_(SK, Bratislava, Europe, Slovakia, Bratislava, 1, 1)                                                                      \
    X3_(SI, Ljubljana, Europe, Slovenia, Ljubljana, 1, 1)                                                                        \
    X3_(ES, Madrid, Europe, Spain, Madrid, 1, 1)                                                                                 \
    X3_(SJ, Longyearbyen, Europe, Svalbard, Longyearbyen, 1, 1)                                                                  \
    X3_(SE, Stockholm, Europe, Sweden, Stockholm, 1, 1)                                                                          \
    X3_(CH, Zurich, Europe, Switzerland, Zürich, 1, 1)                                                                           \
    X3_(TR, Istanbul, Europe, Turkey, Istanbul, 3, 0)                                                                            \
    X3_(UA, Kiev, Europe, Ukraine, Kiev, 2, 1)                                                                                   \
    X3_(RU, Simferopol, Europe, Ukraine, Simferopol, 3, 0)                                                                       \
    X3_(GB, London, Europe, United Kingdom, London, 0, 1)                                                                        \
    X3_(VA, Vatican, Europe, Vatican, Vatican, 1, 1)                                                                             \
    X3_(AX, Mariehamn, Europe, Åland, Mariehamn, 2, 1)                                                                           \
    X3_(IO, Chagos, Indian, Chagos, Diego Garcia, 6, 0)                                                                          \
    X3_(CC, Cocos, Indian, Cocos Island, Bantam, 6.5, 0)                                                                         \
    X3_(TF, Kerguelen, Indian, Kerguelen Isl., Port - aux - Français, 5, 0)                                                      \
    X3_(MV, Maldives, Indian, Maldives, Malé, 5, 0)                                                                              \
    X3_(MU, Mauritius, Indian, Mauritius, Port Louis, 4, 0)                                                                      \
    X3_(RE, Reunion, Indian, Réunion, Saint - Denis, 4, 0)                                                                       \
    X3_(SC, Mahe, Indian, Seychelles, Mahé, 4, 0)                                                                                \
    X3_(UT, M1200, UTC, UTC, UTC - 12 : 00, -12, 0)                                                                              \
    X3_(UT, M1100, UTC, UTC, UTC - 11 : 00, -11, 0)                                                                              \
    X3_(UT, M1000, UTC, UTC, UTC - 10 : 00, -10, 0)                                                                              \
    X3_(UT, M0930, UTC, UTC, UTC - 09 : 30, -9.5, 0)                                                                             \
    X3_(UT, M0900, UTC, UTC, UTC - 09 : 00, -9, 0)                                                                               \
    X3_(UT, M0800, UTC, UTC, UTC - 08 : 00, -8, 0)                                                                               \
    X3_(UT, M0700, UTC, UTC, UTC - 07 : 00, -7, 0)                                                                               \
    X3_(UT, M0600, UTC, UTC, UTC - 06 : 00, -6, 0)                                                                               \
    X3_(UT, M0500, UTC, UTC, UTC - 05 : 00, -5, 0)                                                                               \
    X3_(UT, M0400, UTC, UTC, UTC - 04 : 00, -4, 0)                                                                               \
    X3_(UT, M0330, UTC, UTC, UTC - 03 : 30, -3.5, 0)                                                                             \
    X3_(UT, M0300, UTC, UTC, UTC - 03 : 00, -3, 0)                                                                               \
    X3_(UT, M0200, UTC, UTC, UTC - 02 : 00, -2, 0)                                                                               \
    X3_(UT, M0100, UTC, UTC, UTC - 01 : 00, -1, 0)                                                                               \
    X3_(UT, P0000, UTC, UTC, UTC + 00 : 00, 0, 0)                                                                                \
    X3_(UT, P0100, UTC, UTC, UTC + 01 : 00, 1, 0)                                                                                \
    X3_(UT, P0200, UTC, UTC, UTC + 02 : 00, 2, 0)                                                                                \
    X3_(UT, P0300, UTC, UTC, UTC + 03 : 00, 3, 0)                                                                                \
    X3_(UT, P0330, UTC, UTC, UTC + 03 : 30, 3.5, 0)                                                                              \
    X3_(UT, P0400, UTC, UTC, UTC + 04 : 00, 4, 0)                                                                                \
    X3_(UT, P0430, UTC, UTC, UTC + 04 : 30, 4.5, 0)                                                                              \
    X3_(UT, P0500, UTC, UTC, UTC + 05 : 00, 5, 0)                                                                                \
    X3_(UT, P0530, UTC, UTC, UTC + 05 : 30, 5.5, 0)                                                                              \
    X3_(UT, P0545, UTC, UTC, UTC + 05 : 45, 5.75, 0)                                                                             \
    X3_(UT, P0600, UTC, UTC, UTC + 06 : 00, 6, 0)                                                                                \
    X3_(UT, P0630, UTC, UTC, UTC + 06 : 30, 6.5, 0)                                                                              \
    X3_(UT, P0700, UTC, UTC, UTC + 07 : 00, 7, 0)                                                                                \
    X3_(UT, P0800, UTC, UTC, UTC + 08 : 00, 8, 0)                                                                                \
    X3_(UT, P0830, UTC, UTC, UTC + 08 : 30, 8.5, 0)                                                                              \
    X3_(UT, P0845, UTC, UTC, UTC + 08 : 45, 8.75, 0)                                                                             \
    X3_(UT, P0900, UTC, UTC, UTC + 09 : 00, 9, 0)                                                                                \
    X3_(UT, P0930, UTC, UTC, UTC + 09 : 30, 9.5, 0)                                                                              \
    X3_(UT, P1000, UTC, UTC, UTC + 10 : 00, 10, 0)                                                                               \
    X3_(UT, P1030, UTC, UTC, UTC + 10 : 30, 10.5, 0)                                                                             \
    X3_(UT, P1100, UTC, UTC, UTC + 11 : 00, 11, 0)                                                                               \
    X3_(UT, P1200, UTC, UTC, UTC + 12 : 00, 12, 0)                                                                               \
    X3_(UT, P1245, UTC, UTC, UTC + 12 : 45, 12.75, 0)                                                                            \
    X3_(UT, P1300, UTC, UTC, UTC + 13 : 00, 13, 0)                                                                               \
    X3_(UT, P1400, UTC, UTC, UTC + 14 : 00, 14, 0)

class TimeZone
{
public:
    // Enum is combined from id and subId (e.g. FI_Helsinki)
    enum Id
    {
#define X2_(id, subId, ...) id##_##subId,
#define X3_(id, subId, ...) id##_##subId,
        FOR_TIMEZONE_X_
#undef X2_
#undef X3_
            COUNT_,
        MAX_ = COUNT_ - 1
    };

    /**
            Get offset vs. UTC time in seconds
            @param id           Time zone id
            @param utcTime      UTC time
            @return             Offset in seconds
    */
    static int32_t getOffset_s(Id id, int64_t utcTime);

    /**
            Get offset vs. UTC time on given localized time in seconds
            @param id           Time zone id
            @param localizedTime Localized time
            @return             Offset in seconds

            @note
            If given time is inside daylight saving change period, the
            result is ambiguous.
    */
    inline static int32_t getOffsetByLocalizedTime_s(Id id, int64_t localizedTime)
    {
        return getOffset_s(id, localizedTime - getOffset_s(id, localizedTime));
    }

    /**
            Convert UTC time to localized time
            @param id           Time zone id
            @param utcTime      UTC time
            @return             Localized time
    */
    inline static int64_t convert(Id id, int64_t utcTime) { return utcTime + getOffset_s(id, utcTime); }

    /**
            Get full name of time zone
            @param id           Time zone id
            @return             Pointer to the string (e.g. "Europe/Helsinki", null
       terminated)
    */
    static const char* getName(Id id);

    /**
            Get 2 character long name of time zone
            @param id           Time zone id
            @return             Pointer to the string (e.g. "FI", null terminated)
    */
    static const char* getShortName(Id id);

    /**
            Get full name or part of the name of time zone
            @param id           Time zone id
            @param len          Destination for string length
            @param part         Which part of the name to get:
                                                    0 = full name   (e.g. for id==FI_Helsinki -->
       "Europe/Helsinki")
                                                    1 = first part  (e.g. for id==FI_Helsinki -->
       "Europe")
                                                    2 = second part (e.g. for id==FI_Helsinki -->
       "Helsinki")
                                                    3 = third part  (e.g. for id==FI_Helsinki -->
       "")
            @return             Pointer to the string (not null terminated)

            @note
            Examples for id==FI
                    size_t len;
                    const char *name;
                    name = getName( FI, &len );     // --> name points to "Europe/Helsinki" and
       len is 15
                    name = getName( FI, &len, 1 );  // --> name points to "Europe" and len is 6
                    name = getName( FI, &len, 2 );  // --> name points to "Helsinki" and len is
       8
                    name = getName( FI, &len, 3 );  // --> name points to "" and len is 0
    */
    static const char* getName(Id id, size_t* len, uint32_t partNumber = 0);

    /**
            Get count of time zone name parts
            @param id           Time zone id
            @return             Count (2..3)

            @note
            For example:
                    America/North Dakota/Beulah --> 3 parts
                    Europe/Helsinki             --> 2 parts
    */
    static uint32_t getNamePartCount(Id id);

    /**
    it converts Name (FI_Helsinki) to Id enum
    @param           const char* tzName
    @param           bool *found
    @return             Id
    */
    static TimeZone::Id convertNametoId(const char* tzName, bool* found);
};

} // namespace nea
