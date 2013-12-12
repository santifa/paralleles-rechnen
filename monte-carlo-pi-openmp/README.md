Implementierungsaufwand
=====

Der Aufwand für die Implementierung im Gegensatz zu
pthreads ist sehr einfac, da die Pragmas von OpenMP
einfach und logisch anwendbar sind.

Zufallsgenerator
=====

Der Zufallsgenerator ist nicht Threadsafe darum habe ich
mich für das Critical-Paragma entschieden. Es kennzeichnet
ein Abschnitt als kritisch, also nur ein Thread darf ihn
gleichzeitig betreten. Eine direkte Skalierung liegt in dem
Bereich nicht vor, aber es können unendlich viele Threads
mit diesem Prinzip arbeiten. Irgendwann behindern sich die Threads,
da sie auf einander warten. Dann ist eine andere
Implementierungsvariante zu wählen.
