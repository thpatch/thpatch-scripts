divert(-1)
define(`params', `ifdef(`$1',
	`define(`rsrc_base',`$2')
	define(`rsrc_size',`$3')
	define(`fixup1',`$4')
	define(`fixup2',`$5')
	define(`module_reg',`$6')
	define(`drmp_v$7')')')
define(`drmp', `ifdef(`$1',
	`define(`drmp_addr',eval($2,16))
	define(`drmp_disp',eval($3,16,2))')')
dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl
params(`params',         ,        ,       ,       ,    ,  ) dnl default case
params(`th16',    0xE1000,  0x17F8, -0x8A0, -0xF7F, ebx, 3)
params(`th165',  0x110000,  0x1808,   0x10,   0x10, eax, 1)
params(`th143',  0x143000,  0x1550,   0x10,   0x10, eax, 2)
params(`th15',   0x123000,  0x1758,   0x10,   0x10, eax, 2)
params(`th17tr', 0x127000,  0x13F8,    0x0,    0x0, eax, 2)
params(`th13',    0xE7000,  0x1540,    0x0,    0x0, eax, 3)
params(`th14',    0xFF000,  0x1548,    0x0,    0x0, eax, 3)
params(`th125',   0xDB000,  0x1544,    0x0,    0x0, eax, 3)
params(`th128',   0xDD000,  0x1540,    0x0,    0x0, eax, 3)
params(`th17',   0x12B000,  0x13F8,    0x0,    0x0, eax, 3)
params(`th10',    0x9A000,  0x12E0,    0x0,    0x0, eax, 3)
params(`th11',    0xCB000,  0x1540,    0x0,    0x0, eax, 3)
params(`th12',    0xD7000,  0x1540,    0x0,    0x0, eax, 3)
params(`th18',   0x172000,  0x1BF0,    0x0,    0x0, eax, 4)
params(`th095',   0xE7000,  0x1160,    0x0,    0x0, eax, 4)
params(`th185',  0x17B000,  0x1C00,    0x0,    0x0, eax, 5)
params(`th19',   0x21B000, 0x14F24,    0x0,    0x0, eax, 5)
dnl
params(`th08',  0x14DB000,   0xD60,       ,       , eax,  )
params(`th07',   0xF66000,   0x960,       ,       , eax,  )
params(`th06',   0x2E8000,   0x960,       ,       , eax,  )
dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl
drmp(`drmp_v1', 0x6780, 0x13)
drmp(`drmp_v2', 0x6780, 0x0B)
drmp(`drmp_v3', 0x66D0, 0x13) dnl 3d12683b7333bf386de6293c3104e302
drmp(`drmp_v4', 0x3EA0, 0x0B) dnl 9efaf598c56172deffb12919e1dd5035 719dfa70529f71adcda2fb5a7476ef12
drmp(`drmp_v5', 0x3E70, 0x0B) dnl 8795444ebf02dbaad5dc2d76b8243eb5 c49397e378fb21b77f14a64cc0962ab3
dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl
define(`rsrc_size',((rsrc_size+0xFFF)&~0xFFF))
define(`ofs1',(0x1413+fixup1))
define(`ofs2',(0x2036+fixup2))
define(`ofs3',0x333)
define(`addr',`eval(rsrc_base+rsrc_size+$1,16)')
divert(0)dnl
`	"init_stages": [
		{
			"binhacks": {
				"steamstub_crack": {
					"title": "Crack SteamStub by disabling its integrity check",
					"addr": "Rx'addr(ofs1)`",
					"code": "eb",
					"expected": "74"
				}
			},
			"breakpoints": {
				"init_next_stage#1": {
					"addr": "Rx'addr(ofs2)`",
					"module": "'module_reg`",
					"cavesize": 6
				},
				"init_next_stage#game": {
					"addr": "Rx'addr(ofs3)`",
					"cavesize": 5
				}
			}
		},
		{
			"The addresses here are relative to": "SteamDRMP.dll",
			"binhacks": {
				"steamdrm_crack": {
					"title": "Crack SteamDRM: Remove all communication with the Steam client",
					"addr": "Rx'drmp_addr`",
					"code": "90909090909090909090 b030 8845'drmp_disp` e9"
				}
			}
		}
	]'
