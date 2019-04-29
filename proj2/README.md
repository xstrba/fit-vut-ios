# 2. Úloha IOS (2018/19)

## A. Popis úlohy

## Implementujte v jazyce C modifikovaný synchronizaˇcní problém River Crossing Problem (viz kniha

## The Little Book of Semaphores).

## Naˇrece je jedna vyhlídková lod’. Lod’ muže vézt práv ̊ ˇe jednu skupinu osob, skupinu tvoˇrí právˇeˇctyˇri

## osoby. Nelze tedy vézt více ani ménˇe osob než právˇeˇctyˇri. Osoby jsou rozdˇeleny do dvou kategorií -

## první kategorie jsou osoby, které pracují v prostˇredí operaˇcního systému Linux ( hackers ) a druhou

## kategorii tvoˇrí osoby, které pracují v prostˇredí operaˇcního systému Windows ( serfs ). Platí, že nelze

## vézt skupinu osob, kde je pouze jedna osoba ze stejné kategorie (tˇri hackers a jeden serf nebo jeden

## hacker a tˇri serfs); v takovém pˇrípadˇe se tato osoba necítí bezpeˇcnˇe. Povolené jsou tedy následující tˇri

## kombinace: 4 hackers; 4 serfs; 2 hackers a 2 serfs.

## Osoby, které se chtˇejí pˇrepravit, se shromažd’ují na molu. Molo má omezenou kapacitu, pokud se

## novˇe pˇríchozí osoba již na molo nevejde, odchází pryˇc a zkusí pˇrijít na molo pozdˇeji. Tento postup

## se v pˇrípadˇe potˇreby opakuje, tj. každá osoba nakonec odpluje. Jakmile je možné na molu vytvoˇrit

## vhodnou skupinu, nastoupí tato skupina na lod’ a odpluje. Jedna z osob, která nastoupila na lod’, se

## stává kapitánem aˇrídí lod’. Po urˇcité dobˇe se lod’ vrátí zpˇet, skupina osob vystoupí a odejde pryˇc. Po

## dobu plavby (vˇcetnˇe vylodˇení skupiny) nevstupuje žádná osoba z mola na lod’, osoby však mohou

## pˇricházet na molo. Poˇradí nalodˇení osob není definováno. Kapitán opouští lod’ jako poslední, poˇradí

## vylodˇení ostatních osob není definováno.

## B. Podrobná specifikace úlohy

## Spuštˇení

```
$./proj2 PH SRWC
```
## kde

## • Pje poˇcet osob generovaných v každé kategorii; bude vytvoˇrenoP hackers aP serfs.

##### P >= 2 && (P % 2) == 0

## • Hje maximální hodnota doby (v milisekundách), po které je generován nový proces hackers.

## H >= 0 && H <= 2000.

## • Sje maximální hodnota doby (v milisekundách), po které je generován nový proces serfs.

## S >= 0 && S <= 2000.

## • Rje maximální hodnota doby (v milisekundách) plavby.

## R >= 0 && R <= 2000.

## • Wje maximální hodnota doby (v milisekundách), po které se osoba vrací zpˇet na molo (pokud

## bylo pˇred tím molo plné).

## W >= 20 && W <= 2000.

## • Cje kapacita mola.C >= 5.

## • Všechny parametry jsou celáˇcísla.


### Implementaˇcní detaily

- _Pracujte s procesy, ne s vlákny._
- Hlavní proces vytváˇrí ihned po spuštˇení dva pomocné procesy pro generování proces ̊u osob

### stejné kategorie. Potéˇceká na ukonˇcení všech proces ̊u, které aplikace vytváˇrí. Jakmile jsou tyto

### procesy ukonˇceny, ukonˇcí se i hlavní proces s kódem (exit code) 0.

- Generování procesu ̊
    **-** _hacker_ : pomocný proces generuje procesy pro hackers; každý nový proces je generován

### po uplynutí náhodné doby z intervalu<0, H>; celkem vygenerujePprocesu. Pokud platí ̊

### H==0, všechny pˇríslušné procesy se vygenerují ihned.

**-** _serf_ : pomocný proces generuje procesy pro serfs; každý nový proces je generován po uply-

### nutí náhodné doby z intervalu<0, S>; celkem vygenerujePproces ̊u. Pokud platíS==0,

### všechny pˇríslušné procesy se vygenerují ihned.

**-** Každý proces bude internˇe identifikován celýmˇcíslemI, zaˇcínajícím od 1 .ˇCíselnáˇrada je

### pro každou kategorii procesu zvlášt’. ̊

**-** Postupnˇe tedy vznikne hlavní proces, dva pomocné procesy a2*Pprocesu osob. ̊
- Každý proces vykonává své akce a souˇcasnˇe zapisuje informace o akcích do souboru s názvem

### proj2.out. Souˇcástí výstupních informací o akci je poˇradovéˇcísloAprovádˇené akce (viz popis

### výstup ̊u). Akce seˇcíslují od jedniˇcky.

- Použijte sdílenou pamˇet’ pro implementaciˇcítaˇce akcí a sdílených promˇenných nutných pro syn-

### chronizaci.

- Použijte semafory pro synchronizaci proces ̊u.
- _Nepoužívejte aktivní ˇcekání (vˇcetnˇe cyklického ˇcasového uspání procesu) pro úˇcely synchronizace._

### Chybové stavy

- Pokud nˇekterý ze vstup ̊u nebude odpovídat oˇcekávanému formátu nebo bude mimo povolený

### rozsah, program vytiskne chybové hlášení na standardní chybový výstup,uvolní všechny dosud

### alokované zdroje a ukonˇcí se s kódem (exit code) 1.


### Popis procesu a jejich výstup ̊ u ̊

### Poznámka k výstup ̊um

- Aje poˇradovéˇcíslo provádˇené akce,
- NAMEje zkratka kategorie pˇríslušného procesu, tj.HACKneboSERF,
- Ije interní identifikátor procesu v rámci pˇríslušné kategorie,
- NHje aktuální poˇcet hackers na molu a
- NSje aktuální poˇcet serfs na molu.
- Pˇri vyhodnocování výstupu budou ignorovány mezery a tabelátory.

### Proces osoby ( hacker i serf )

### 1. Po spuštˇení tiskneA: NAME I: starts

### 2. Pokouší se dostat na molo

### (a) Pokud je molo plné (kapacita je naplnˇena), uspí se proces na náhodnou dobu z intervalu

### <20, W>; pˇred uspáním tiskneA: NAME I: leaves queue: NH: NS. Po vzbuzení tiskne

### A: NAME I: is ba
ka pokusí se opˇet vstoupit na molo (opakuje bod 2).

### (b) Pokud m ̊uže vstoupit na molo, tiskneA: NAME I: waits: NH: NS. Poznámka k této va-

### riantˇe: Hodnoty NH a NS zahrnují i tento proces, který právˇe vstoupil na molo.

### 3. Jakmile je možné vytvoˇrit vhodnou skupinu:

### (a)Ctyˇ ˇri procesy vstoupí na lod’, jeden z nich se stává kapitánem.

### (b) Kapitán pˇri vstupu tiskneA: NAME I: boards: NH: NS; ostatní procesy netisknou nic.

### (c) Nástup osob probˇehne z pohledu ostatních proces ̊u atomicky, tj. poˇcet osob na molu se sníží

### o 4 v jednom okamžiku; tento stav je garantován po tisku kapitána.

### (d) Poznámka k této variantˇe: Hodnoty NH a NS reflektují stav po nalodˇení všech osob.

### 4. Plavba je simulována uspáním procesu kapitána na náhodnou dobu z intervalu<0, R>. Ostatní

### ˇclenové posádky (members)ˇcekají na vzbuzení procesu kapitána.

### 5. Po ukonˇcení plavby a vylodˇení tiskne každýˇclen posádky (kromˇe kapitána)

### A: NAME I: member exits: NH: NS.

### Kapitán opouští lod’ jako poslední a tiskne

### A: NAME I: 
aptain exits: NH: NS.

### 6. Proces odchází pryˇc a ukonˇcí se.

### 7. Další osoby mohou vstoupit na lod’ (bod 3) až poté, co poslední osoba zeˇctveˇrice (kapitán)

### opustí lod’ a vytiskne pˇríslušnou informaci (bod 5).


## C. Podmínky vypracování

### Obecné informace

- Projekt implementujte v jazyce C. Komentujte zdrojové kódy, programujte pˇrehlednˇe. Souˇcástí

### hodnocení bude i kvalita zdrojového kódu.

- Kontrolujte, zda se všechny procesy ukonˇcují korektnˇe a zda pˇri ukonˇcování správnˇe uvolˇnujete

### všechny alokované zdroje.

- Dodržujte syntax zadaných jmen, formát souboru a formát výstupních dat. Použijte základní ̊

### skript pro ovˇeˇrení korektnosti výstupního formátu (dostupný z webu se zadáním). Informace o

### skriptu jsou uvedeny v komentáˇri skriptu.

- Dotazy k zadání: Veškeré nejasnosti a dotazyˇrešte pouze prostˇrednictvím diskuzního fóra k

### projektu 2.

### Pˇreklad

- Pro pˇreklad používejte nástrojmake. Souˇcástí odevzdání bude souborMakefile.
- Pˇreklad se provede pˇríkazemmakev adresáˇri, kde je umístˇen souborMakefile.
- Po pˇrekladu vznikne spustitelný soubor se jménemproj2, který bude umístˇen ve stejném adre-

### sáˇri jako souborMakefile

### • Zdrojové kódy pˇrekládejte s pˇrepínaˇci-std=gnu99 -Wall -Wextra -Werror -pedanti

- Pokud to vašeˇrešení vyžaduje, lze pˇridat další pˇrepínaˇce pro linker (napˇr. kvuli semafor ̊ ̊um).

### Odevzdání

### • Souˇcástí odevzdání budou pouze soubory se zdrojovými kódy (*.
,*.h) a souborMakefile.

### Tyto soubory zabalte pomocí nástoje zip do archivu s názvemproj2.zip.

- Archiv vytvoˇrte tak, aby po rozbalení byl souborMakefileumístˇen ve stejném adresáˇri, jako je

### archiv.

- Archivproj2.zipodevzdejte prostˇrednictvím informaˇcního systému, termín _Projekt 2_.
- Pokud nebude dodržena forma odevzdání nebo projekt nep ̊ujde pˇreložit, bude projekt hodnocen

### 0 body.

- Archiv odevzdejte pomocí informaˇcního systému v dostateˇcném pˇredstihu (odevzdaný soubor

### mužete p ̊ ˇred vypršením termínu snadno nahradit jeho novˇejší verzí, kdykoliv budete potˇrebo-

### vat).


## D. Ukázka výstupu ̊

### Ukázka 1

### Spuštˇení $./proj2 22 2200200 5

### Výstup

```
1 : HACK 1 :starts
2 : HACK 1 :waits : 1 : 0
3 : SERF 1 :starts
4 : SERF 1 :waits : 1 : 1
5 : HACK 2 :starts
6 : SERF 2 :starts
7 : HACK 2 :waits : 2 : 1
8 : SERF 2 :waits : 2 : 2
9 : SERF 2 :boards : 0 : 0
10 : HACK 2 : memberexits : 0 : 0
11 : SERF 1 : memberexits : 0 : 0
12 : HACK 1 : memberexits : 0 : 0
13 : SERF 2 : 
aptainexits : 0 : 0
```

### Ukázka 2

#### Spuštˇení $ ./proj2 60 02002005

#### Výstup

```
1 : HACK 1 :starts
2 : HACK 1 :waits : 1 : 0
3 : SERF 1 :starts
4 : SERF 1 :waits : 1 : 1
5 : HACK 2 :starts
6 : HACK 2 :waits : 2 : 1
7 : SERF 2 :starts
8 : SERF 2 :waits : 2 : 2
9 : SERF 5 :starts
10 : HACK 6 : starts
11 : HACK 3 : starts
12 : SERF 6 : starts
13 : SERF 3 : starts
14 : SERF 2 : boards : 0 : 0
15 : SERF 5 : waits : 0 : 1
16 : HACK 6 : waits : 1 : 1
17 : HACK 3 : waits : 2 : 1
18 : SERF 6 : waits : 2 : 2
19 : SERF 3 : waits : 2 : 3
20 : HACK 4 : starts
21 : HACK 4 : leavesqueue : 2 : 3
22 : SERF 4 : starts
23 : SERF 4 : leavesqueue : 2 : 3
24 : HACK 5 : starts
25 : HACK 5 : leavesqueue : 2 : 3
26 : HACK 2 : memberexits : 2 : 3
27 : SERF 1 : memberexits : 2 : 3
28 : HACK 1 : memberexits : 2 : 3
29 : HACK 4 : isba
k
30 : HACK 4 : leavesqueue : 2 : 3
31 : SERF 4 : isba
k
32 : SERF 4 : leavesqueue : 2 : 3
33 : SERF 2 : 
aptainexits : 2 : 3
34 : SERF 6 : boards : 0 : 1
35 : HACK 5 : isba
k
36 : HACK 5 : waits : 1 : 1
37 : HACK 4 : isba
k
38 : HACK 4 : waits : 2 : 1
39 : SERF 4 : isba
k
40 : SERF 4 : waits : 2 : 2
41 : HACK 3 : memberexits : 2 : 2
42 : HACK 6 : memberexits : 2 : 2
43 : SERF 5 : memberexits : 2 : 2
44 : SERF 6 : 
aptainexits : 2 : 2
45 : SERF 4 : boards : 0 : 0
46 : HACK 5 : memberexits : 0 : 0
47 : HACK 4 : memberexits : 0 : 0
48 : SERF 3 : memberexits : 0 : 0
49 : SERF 4 : 
aptainexits : 0 : 0
```


