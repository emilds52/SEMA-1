ltl p1 { [](!(Vp && Vs)) } // Globally never Vp and Vs at the same time
ltl p2 { []<> nxt -> ([](E -> <> Vs)) } // Globally if E then eventually Vp
// RELATED WITH CROSSWALK
ltl p3 { []<> nxt ->( [](Bp -> <> Rp)) } 
ltl p4 { []<> nxt ->( [](Bs -> <> Rs)) }
ltl p5 { [](!(Vp && SPp)) } // Globally never SPp and Vp at the same time
ltl p6 { [](!(Vs && SPs)) } // Globally never SPs and Vs at the same time

int E;
int nxt;
//Rp=Rojo principal, Ap=Amarillo principal, Vp=Verde principal
//Bp=Boton principal, Spp=Semaforo peatones principal -> binario, con 1 es verde, con 0 es rojo.
int Rp, Ap, Vp, Bp, SPp;
int Rs, As, Vs, Bs, SPs;
int state;

mtype = { s_Vp, s_Ap, s_Rp, s_Vs, s_As, s_Rs };

active proctype fsm() {
	state = s_Vp;
	Vp = 1; Ap = 0; Rp = 0; 
	Vs = 0; As = 0; Rs = 1; 
	SPp = Rp; SPs = Rs;
	nxt = 0;
	do
	:: (state == s_Vp) -> atomic {
		if
		:: (nxt && (E || Bp)) -> Ap = 1; Vp = 0; nxt = 0; E = 0; Bp = 0; state = s_Ap;
		fi
	}
	:: (state == s_Ap) -> atomic {
		if
		:: nxt -> Ap = 0; Rp = 1; nxt = 0; state = s_Rp; SPp = 1;
		fi
	}
	:: (state == s_Rp) -> atomic {
		if
		:: nxt -> Vs = 1; Rs = 0;  nxt = 0; state = s_Vs; SPs = 0;
		fi
	}
	:: (state == s_Vs) -> atomic {
		if
		:: (nxt && (!E || Bs))-> As = 1; Vs = 0; Bs = 0; nxt = 0; state = s_As;  // El boton tiene prioridad sobre la espira
		fi
	}
	:: (state == s_As) -> atomic {
		if
		:: nxt -> As = 0; Rs = 1; nxt = 0; state = s_Rs; SPs = 1;
		fi
	}
	:: (state == s_Rs) -> atomic {
		if
		:: nxt -> Vp = 1; Rp = 0; nxt = 0; state = s_Vp; SPp = 0;
		fi
	}
	od
}

active proctype entorno () {
	do
	:: if
		:: E = 1
		:: Bp = 0
		:: Bs = 0
		:: nxt = 1
		:: skip
		fi;
		printf("state = %e, E = %d, next = %d, BotPrin = %d, BotSec = %d\n", state, E, nxt, Bp, Bs)
	od
}
