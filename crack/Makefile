SHELL=/bin/bash
DIFF=diff -u --color=always
SEDFILT=sed -n '$$d;/"init_stages"/,$$p'
.PHONY:check
check:
	@-echo -------------------------------- th16
	@-$(DIFF) <(m4 -Dth16 crack.m4) <($(SEDFILT) nmlgc/base_tsa/th16.v1.00a.js)
	@-echo -------------------------------- th165
	@-$(DIFF) <(m4 -Dth165 crack.m4) <($(SEDFILT) nmlgc/base_tsa/th165.v1.00a.js)
	@-echo -------------------------------- th143
	@-$(DIFF) <(m4 -Dth143 crack.m4) <($(SEDFILT) nmlgc/base_tsa/th143.v1.00a.js)
	@-echo -------------------------------- th15
	@-$(DIFF) <(m4 -Dth15 crack.m4) <($(SEDFILT) nmlgc/base_tsa/th15.v1.00b.js)
	@-echo -------------------------------- th17tr
	@-$(DIFF) <(m4 -Dth17tr crack.m4) <($(SEDFILT) nmlgc/base_tsa/th17.v0.01a.js)
	@-echo -------------------------------- th13
	@-$(DIFF) <(m4 -Dth13 crack.m4) <($(SEDFILT) nmlgc/base_tsa/th13.v1.00c.js)
	@-echo -------------------------------- th14
	@-$(DIFF) <(m4 -Dth14 crack.m4) <($(SEDFILT) nmlgc/base_tsa/th14.v1.00b.js)
	@-echo -------------------------------- th125
	@-$(DIFF) <(m4 -Dth125 crack.m4) <($(SEDFILT) nmlgc/base_tsa/th125.v1.00a.js)
	@-echo -------------------------------- th128
	@-$(DIFF) <(m4 -Dth128 crack.m4) <($(SEDFILT) nmlgc/base_tsa/th128.v1.00a.js)
	@-echo -------------------------------- th17
	@-$(DIFF) <(m4 -Dth17 crack.m4) <($(SEDFILT) nmlgc/base_tsa/th17.v1.00b.js)
	@-echo -------------------------------- th10
	@-$(DIFF) <(m4 -Dth10 crack.m4) <($(SEDFILT) nmlgc/base_tsa/th10.v1.00a.js)
	@-echo -------------------------------- th11
	@-$(DIFF) <(m4 -Dth11 crack.m4) <($(SEDFILT) nmlgc/base_tsa/th11.v1.00a.js)
	@-echo -------------------------------- th12
	@-$(DIFF) <(m4 -Dth12 crack.m4) <($(SEDFILT) nmlgc/base_tsa/th12.v1.00b.js)
	@-echo -------------------------------- th18
	@-$(DIFF) <(m4 -Dth18 crack.m4) <($(SEDFILT) nmlgc/base_tsa/th18.v1.00a.js)
	@-echo -------------------------------- th095
	@-$(DIFF) <(m4 -Dth095 crack.m4) <($(SEDFILT) nmlgc/base_tsa/th095.v1.02a.js)
	@-echo -------------------------------- th185
	@-$(DIFF) <(m4 -Dth185 crack.m4) <($(SEDFILT) nmlgc/base_tsa/th185.v1.00a.js)
	@-echo -------------------------------- th19 v1.00a
	@-$(DIFF) <(m4 -Dth19 crack.m4) <($(SEDFILT) nmlgc/base_tsa/th19.v1.00a.js)
	@-echo -------------------------------- th19 v1.10c
	@-$(DIFF) <(m4 -Dth19a crack.m4) <($(SEDFILT) nmlgc/base_tsa/th19.v1.10c.js)
