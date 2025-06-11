changequote([,])dnl
#!/usr/bin/env bash
dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl
define([GAMES])dnl
define([GAMEPARAMS], [dnl
define([GAMES],GAMES[ $1])dnl
define([BIND_BASE],[($2+(($3+0xFFF)&~0xFFF))])dnl
		$1) dnl
ofs1="eval(BIND_BASE+$4, 16)" dnl
ofs2="eval(BIND_BASE+$5, 16)" dnl
ofs3="eval(BIND_BASE+$6, 16)" dnl
module_reg="$7" dnl
jsfile="$9"; dnl
setdrmpparams "$8" ;;])dnl
function setgameparams {
	case "$1" in
dnl        $1            $2       $3      $4      $5     $6   $7 $8  $9
dnl        name    rsrc_base   _size    ofs1    ofs2 ofs3 modreg drmp jsfile
GAMEPARAMS(th16,    0xE1000,  0x17F8,  0xB73, 0x10B7, 0x333, ebx, 3, th16.v1.00a.js)
GAMEPARAMS(th165,  0x110000,  0x1808, 0x1423, 0x2046, 0x333, eax, 1, th165.v1.00a.js)
GAMEPARAMS(th143,  0x143000,  0x1550, 0x1423, 0x2046, 0x333, eax, 2, th143.v1.00a.js)
GAMEPARAMS(th15,   0x123000,  0x1758, 0x1423, 0x2046, 0x333, eax, 2, th15.v1.00b.js)
GAMEPARAMS(th17tr, 0x127000,  0x13F8, 0x1413, 0x2036, 0x333, eax, 2, th17.v0.01a.js)
GAMEPARAMS(th13,    0xE7000,  0x1540, 0x1413, 0x2036, 0x333, eax, 3, th13.v1.00c.js)
GAMEPARAMS(th14,    0xFF000,  0x1548, 0x1413, 0x2036, 0x333, eax, 3, th14.v1.00b.js)
GAMEPARAMS(th125,   0xDB000,  0x1544, 0x1413, 0x2036, 0x333, eax, 3, th125.v1.00a.js)
GAMEPARAMS(th128,   0xDD000,  0x1540, 0x1413, 0x2036, 0x333, eax, 3, th128.v1.00a.js)
GAMEPARAMS(th17,   0x12B000,  0x13F8, 0x1413, 0x2036, 0x333, eax, 3, th17.v1.00b.js)
GAMEPARAMS(th10,    0x9A000,  0x12E0, 0x1413, 0x2036, 0x333, eax, 3, th10.v1.00a.js)
GAMEPARAMS(th11,    0xCB000,  0x1540, 0x1413, 0x2036, 0x333, eax, 3, th11.v1.00a.js)
GAMEPARAMS(th12,    0xD7000,  0x1540, 0x1413, 0x2036, 0x333, eax, 3, th12.v1.00b.js)
GAMEPARAMS(th18,   0x172000,  0x1BF0, 0x1413, 0x2036, 0x333, eax, 4, th18.v1.00a.js)
GAMEPARAMS(th095,   0xE7000,  0x1160, 0x1413, 0x2036, 0x333, eax, 4, th095.v1.02a.js)
GAMEPARAMS(th185,  0x17B000,  0x1C00, 0x1413, 0x2036, 0x333, eax, 5, th185.v1.00a.js)
GAMEPARAMS(th19,   0x21B000, 0x14F24, 0x1413, 0x2036, 0x333, eax, 5, th19.v1.00a.js)
GAMEPARAMS(th19a,  0x23C000, 0x17788, 0x1413, 0x2036, 0x333, eax, 6, th19.v1.10c.js)
dnl GAMEPARAMS(th08,  0x14DB000,   0xD60,       ,       ,      , eax,  , th08.v1.00d.js)
dnl GAMEPARAMS(th07,   0xF66000,   0x960,       ,       ,      , eax,  , th07.v1.00b.js)
dnl GAMEPARAMS(th06,   0x2E8000,   0x960,       ,       ,      , eax,  , th06.v1.02h.js)
		*)
			echo "unknown game $1"
			exit 1
			;;
	esac
}
dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl
define([DRMPPARAMS],[dnl
		$1) drmp_addr="eval($2,16)" drmp_disp="eval($3,16,2)" ;;])dnl
function setdrmpparams {
	case "$1" in
DRMPPARAMS(1, 0x6780, 0x13)
DRMPPARAMS(2, 0x6780, 0x0B)
DRMPPARAMS(3, 0x66D0, 0x13) # 3d12683b7333bf386de6293c3104e302
DRMPPARAMS(4, 0x3EA0, 0x0B) # 9efaf598c56172deffb12919e1dd5035 719dfa70529f71adcda2fb5a7476ef12
DRMPPARAMS(5, 0x3E70, 0x0B) # 8795444ebf02dbaad5dc2d76b8243eb5 c49397e378fb21b77f14a64cc0962ab3
DRMPPARAMS(6, 0x3D50, 0x0B) # bcca22165ad6908348793ca4511086cc
		*)
			echo "unknown drmp $1"
			exit 1
			;;
	esac
}
dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl
define([MAP],[ifelse([$2],[()],[],[$1$2[]$0([$1],(shift$2))])])dnl
define([SUBST],[
		s/@$1@/${$1}/g])dnl
function configure {
	sed "MAP([SUBST], (ofs1, ofs2, ofs3, module_reg, drmp_addr, drmp_disp))"
}
dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl
function sedfilt {
	sed -n '$d;/"init_stages"/,$p'
}
dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl dnl
if test -n "$1"; then
	setgameparams "$1"
	configure <template.json.in
else
	if ! test -d nmlgc; then
		echo "nmlgc directory is missing (should be a clone of <https://github.com/thpatch/thcrap-tsa.git>)"
		exit 1
	fi
	for game in GAMES(); do
		setgameparams "$game"
		echo -------------------------------- $game
		${DIFF:-diff -u --color=always} <(configure <template.json.in) <(sedfilt <nmlgc/base_tsa/$jsfile)
	done
fi
