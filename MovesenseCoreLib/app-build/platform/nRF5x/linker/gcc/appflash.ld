/*
 * application specific linker script (starts at 0X2080 (ram) 0x1f000 (Flash) )
 */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x1f000, LENGTH = 0x55000

  /**
   * Leave 80 bytes to the end of RAM for over reset info (NOINIT)
   */
  RAM (rwx) :  ORIGIN = 0x20002638, LENGTH = 0xD948

  /** Location of non initialized RAM. Non initialized RAM is used for exchanging bond information
   *  from application to bootloader when using buttonluss DFU OTA.
   */
  NOINIT (rwx) :  ORIGIN = 0x2000FF80, LENGTH = 0x80

  /** Location of bootloader setting in flash. */
  BOOTLOADER_SETTINGS (rw) : ORIGIN = 0x0007F000, LENGTH = 0x1000

  /** Location in UICR where bootloader start address is stored. */
  UICR_BOOTLOADER (r) : ORIGIN = 0x10001014, LENGTH = 0x04

  /** Location of mbr params page in flash. */
  MBR_PARAMS_PAGE (rw) : ORIGIN = 0x0007E000, LENGTH = 0x1000

  /** Location in UICR where mbr params page address is stored. */
  UICR_MBR_PARAM_PAGE(r) : ORIGIN = 0x10001018, LENGTH = 0x04

  /** Location in UICR where PSELRESET params settings are stored. */
  UICR_PSELRESET_PAGE(r) : ORIGIN = 0x10001200, LENGTH = 0x08
}

SECTIONS
{
  /* Place the bootloader settings page in flash. */
  .bootloaderSettings(NOLOAD) :
  {

  } > BOOTLOADER_SETTINGS

  /* Write the bootloader address in UICR. */
  .uicrBootStartAddress :
  {
    KEEP(*(.uicrBootStartAddress))
  } > UICR_BOOTLOADER

  /* Place the mbr params page in flash. */
    .mbrParamsPage(NOLOAD) :
  {

  } > MBR_PARAMS_PAGE

  /* Write the bootloader address in UICR. 
  .uicrMbrParamsPageAddress :
  {
    KEEP(*(.uicrMbrParamsPageAddress))
  } > UICR_MBR_PARAM_PAGE
*/

  /* Write RESET pin settings (dissable RESET pin) in UICR */
  .uicrPselresetPage :
  {
    KEEP(*(.uicrPselresetPage))
  } > UICR_PSELRESET_PAGE

  /* No init RAM section. Used for stuff like timestorage & faultcom. */
  .timestorage(NOLOAD) :
  {
  } > NOINIT
  .faultcom(NOLOAD) :
  {
  } > NOINIT

  .fs_data :
  {
    PROVIDE(__start_fs_data = .);
    KEEP(*(.fs_data))
    PROVIDE(__stop_fs_data = .);
  } > RAM
} INSERT AFTER .data;

INCLUDE common.ld
