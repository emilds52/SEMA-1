//TODO: Ahora mismo compila, pero p5, p7 y p8 dice que no las cumple. Depurar esto es una mierda.
//además, si next no se pone a 1, es normal que no se pongan, pero sabemos que siempre se va a poner (paso del tiempo)


ltl p1 { [](!(Vp && Vs)) } // Globally never Vp and Vs at the same time
ltl p2 { []<> (nxt && E) -> []<>Vp } // Globally eventually E means globally eventually Vp (?) --Sergio-- no la comprendo
ltl p3 { []<> nxt -> ([](E -> <> Vs)) } // Globally if E then eventually Vp
ltl p4 { [](Vs && !E -> <>(!Vs W E)) } // Si V de la segunda y no E entonces eventualmente no se pone Vs a no ser que haya E? --Sergio-- no la comprendo
// RELATED WITH CROSSWALK
ltl p5 { []<> nxt ->( [](Bp -> <> Rp)) }
ltl p6 { []<> nxt ->( [](Bs -> <> Rs)) }
ltl p7 { [](!(Vp && SPp)) } // Globally never SPp and Vp at the same time
ltl p8 { [](!(Vs && SPs)) } // Globally never SPs and Vs at the same time

int E;
int nxt;
//Rp=Rojo principal, Ap=Amarillo principal, Vp=Verde principal
//Bp=Boton principal, Spp=Semaforo peatones principal -> binario, con 1 es verde, con 0 es rojo. siempre es igual a Rp (solo cruzamos si no pasan coches)
int Rp, Ap, Vp, Bp, SPp;
int Rs, As, Vs, Bs, SPs;
int state;

mtype = { s_Vp, s_Ap, s_Rp, s_Vs, s_As, s_Rs };

active proctype fsm() {
	state = s_Vp;
	Vp = 1; Ap = 0; Rp = 0; 
	Vs = 0; As = 0; Rs = 1; 
	SPp = Rp; SPs = Rs; // always
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
