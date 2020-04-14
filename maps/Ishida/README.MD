# Ishida Modbus Documentation


## READ TABLE
| Name  | Function | Start Address | Length | Data Type | Byte Order |
| ------- | ------ | ------- | -------- | -------- | ------- |
| Current Weight (divide by 10) | 3 (Read) | 160 | 2 | Unsigned 32-bit Integer | Little Endian |
| Current Preset Selected | 3 (Read) | 523 | 1 | Unsigned 16-bit Integer | Little Endian |
| Reference Weight (divide by 10) | 3 (Read) | 524 | 2 | Unsigned 32-bit Integer | Little Endian |
| Upper Limit (divide by 10) | 3 (Read) | 526 | 2 | Unsigned 32-bit Integer | Little Endian |
| Lower Limit (divide by 10) | 3 (Read) | 528 | 2 | Unsigned 32-bit Integer | Little Endian |
| OK Upper Limit (divide by 10) | 3 (Read) | 530 | 2 | Unsigned 32-bit Integer | Little Endian |
| OK Lower Limit (divide by 10) | 3 (Read) | 532 | 2 | Unsigned 32-bit Integer | Little Endian |
| Nominal Weight (divide by 10) | 3 (Read) | 534 | 2 | Unsigned 32-bit Integer | Little Endian |
| Tolerable Limit (divide by 10) | 3 (Read) | 536 | 2 | Unsigned 32-bit Integer | Little Endian |
| Absolute Tole Limit (divide by 10) | 3 (Read) | 538 | 2 | Unsigned 32-bit Integer | Little Endian |
| Tare (divide by 10) | 3 (Read) | 540 | 2 | Unsigned 32-bit Integer | Little Endian |
| Product Name | 3 (Read) | 542 | 12 | String | Little Endian |
| Total Count | 3 (Read) | 568 | 2 | Unsigned 32-bit Integer | Little Endian |
| Maximum Weight Seen (divide by 10) | 3 (Read) | 570 | 2 | Unsigned 32-bit Integer | Little Endian |
| Minimum Weight Seen | 3 (Read) | 572 | 2 | Unsigned 32-bit Integer | Little Endian |
| Standard Deviation Seen | 3 (Read) | 578 | 2 | Unsigned 32-bit Integer | Little Endian |
| Proper Count | 3 (Read) | 586 | 2 | Unsigned 32-bit Integer | Little Endian |
| Under Count | 3 (Read) | 588 | 2 | Unsigned 32-bit Integer | Little Endian |
| Over Count | 3 (Read) | 590 | 2 | Unsigned 32-bit Integer | Little Endian |
| OK Under Count | 3 (Read) | 592 | 2 | Unsigned 32-bit Integer | Little Endian |
| OK Over Count | 3 (Read) | 594 | 2 | Unsigned 32-bit Integer | Little Endian |
| Pass Count | 3 (Read) | 604 | 2 | Unsigned 32-bit Integer | Little Endian |
| All Count | 3 (Read) | 606 | 2 | Unsigned 32-bit Integer | Little Endian |
| Histogram Base Value (divide by 10) | 3 (Read) | 612 | 2 | Unsigned 32-bit Integer | Little Endian |
| Histogram Interval Value | 3 (read) | 614 | 2 | Unsigned 32-bit Integer | Little Endian |
| Histogram Field (1) | 3 (read) | 616 | 2 | Unsigned 32-bit Integer | Little Endian |
| Histogram Field (2) | 3 (read) | 618 | 2 | Unsigned 32-bit Integer | Little Endian |
| Histogram Field (3) | 3 (read) | 620 | 2 | Unsigned 32-bit Integer | Little Endian |
| Histogram Field (4) | 3 (read) | 622 | 2 | Unsigned 32-bit Integer | Little Endian |
| Histogram Field (5) | 3 (read) | 624 | 2 | Unsigned 32-bit Integer | Little Endian |
| Histogram Field (6) | 3 (read) | 626 | 2 | Unsigned 32-bit Integer | Little Endian |
| Histogram Field (7) | 3 (read) | 628 | 2 | Unsigned 32-bit Integer | Little Endian |
| Histogram Field (8) | 3 (read) | 630 | 2 | Unsigned 32-bit Integer | Little Endian |
| Histogram Field (9) | 3 (read) | 632 | 2 | Unsigned 32-bit Integer | Little Endian |
| Histogram Field (10) | 3 (read) | 634 | 2 | Unsigned 32-bit Integer | Little Endian |
| Histogram Field (11) | 3 (read) | 636 | 2 | Unsigned 32-bit Integer | Little Endian |
| Histogram Field (12) | 3 (read) | 638 | 2 | Unsigned 32-bit Integer | Little Endian |
| Histogram Field (13) | 3 (read) | 640 | 2 | Unsigned 32-bit Integer | Little Endian |
| Histogram Field (14) | 3 (read) | 642 | 2 | Unsigned 32-bit Integer | Little Endian |
| Giveaway Weight | 3 (Read) | 843 | 4 | Unsigned 64-bit Integer | Little Endian |
| Giveaway Weight (positive/negative) | 3 (Read) | 847 | 1 | Bool | Little Endian |
| Giveaway Rate (divide by 1000) | 3 (Read) | 848 | 4 | Unsigned 64-bit Integer | Little Endian |
| Giveaway Rate (positive/negative) | 3 (Read) | 852 | 1 | Bool | Little Endian |

## WRITE TABLE
| Name  | Function | Start Address | Length | Data Type | Byte Order |
| ------- | ------ | ------- | -------- | -------- | ------- |
| System Date | 4 (read/write) | 3 | 3 | Date (YMD) | Little Endian |
| System Time | 4 (read/write) | 7 | 3 | Time (HMS) | Little Endian |
| Preset Number | 4 (read/write) | 500 | 1 | Unsigned 16-bit Integer | Little Endian |
| Reference Weight (divide by 10)| 4 (read/write) | 501 | 2 | Unsigned 32-bit Integer | Little Endian |
| Upper Limit (divide by 10)| 4 (read/write) | 503 | 2 | Unsigned 32-bit Integer | Little Endian |
| Lower Limit (divide by 10)| 4 (read/write) | 505 | 2 | Unsigned 32-bit Integer | Little Endian |
| OK-Upper Limit (divide by 10)| 4 (read/write) | 507 | 2 | Unsigned 32-bit Integer | Little Endian |
| OK-Lower Limit (divide by 10)| 4 (read/write) | 509 | 2 | Unsigned 32-bit Integer | Little Endian |
| Nominal Weight (divide by 10)| 4 (read/write) | 511 | 2 | Unsigned 32-bit Integer | Little Endian |
| TNE Weight (divide by 10)| 4 (read/write) | 513 | 2 | Unsigned 32-bit Integer | Little Endian |
| Product Length | 4 (read/write) | 519 | 1 | Unsigned 16-bit Integer | Little Endian |
| Product Name | 4 (read/write) | 520 | 12 | String | Little Endian |
| Product Code | 4 (read/write) | 532 | 8 | String | Little Endian |
| Max Speed | 4 (read/write) | 542 | 2 | Unsigned 16-bit Integer | Little Endian |
| Over Weight OK Accept Flag | 4 (read/write) | 612 | 1 | Bool | Little Endian |
| Tare Weight (divide by 10) | 4 (read/write) | 614 | 2 | Unsigned 32-bit Integer | Little Endian |
| Unit of Weight [Gram, Ounce] | 4 (read/write) | 642 | 1 | Unsigned 16-bit Integer | Little Endian |
| Assign Preset Number | 4 (read/write) | 997 | 1 | Unsigned 16-bit Integer | Little Endian |
| System Type [DACS-G, DIX-G] | 4 (read/write) | 1091 | 1 | Unsigned 16-bit Integer | Little Endian |
| Scale Type [015, 060] | 4 (read/write) | 1092 | 1 | Unsigned 16-bit Integer | Little Endian |
| Use Weight Log | 4 (read/write) | 1149 | 1 | Bool | Little Endian |
| Auto-Zero Fault | 4 (read/write) | 1275 | 1 | Bool | Little Endian |