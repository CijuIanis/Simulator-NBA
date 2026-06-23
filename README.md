# Simulator NBA

In acest proiect am facut un simulator de sezoane, in opinia mea, interesante, din NBA. Aplicatia permite selectarea unui sezon din cele 13 la dispozitie (ex: 1965-66, 1999-00, 2007-08, 2022-23 etc.).

Fiecare sezon are echipe cu lotul intreg si cu stats-urile jucatorilor adevarate (aceste informatii au fost generate de inteligenta artificiala, deoarece mi-ar fi luat foarte mult timp sa scriu cate 12 jucatori per echipa pentru 9, 17, 23, 27, 29 sau 30 de echipe, depinde de sezon). Pentru a stabili daca un jucator este All-Star sau Role Player am facut o medie a statisticilor, deoarece acest lucru este diferit de la sezon la sezon. (ex: in anul 1965-66 un jucator All-Star ar fi avut stats-uri mai slabe decat unul din sezonul 2022-23. De aceea am pus conditiile: >= 18 PPG || >= 14 PPG && >= 7 APG || >= 14 PPG && >= 8 RPG).

Functionalitati principale:
- Explorarea echipelor si jucatorilor din fiecare sezon;
- Calculul statisticilor individuale (PPG, APG, RPG);
- Identificarea celui mai bun jucator dintr-un sezon = impact score (pentru Tema 1 am calculat prin formula: PPG + APG * 0.75 + RPG * 0.5);
- Determinarea favoritei la titlu pe baza scorului de impact (am folosit aceeasi formula ca la cel mai bun jucator, iar daca un jucator este All-Star conform unor criterii de puncte, pase sau recuperari, rezultatul este inmultit cu 1.5. Favorita la titlu se afla prin suma tuturor scorurilor de impact ale jucatorilor din fiecare echipa);

Am extins simulatorul cu o ierarhie de mosteniri bazata pe pozitiile jucatorilor. Clasa abstracta `Player` are 4 clase derivate: `Guard`, `Forward`, `Center` si `TwoWayPlayer`, fiecare cu propria formula de evaluare prin `calculateRoleScore()`:

- **Guard** — evaluat dupa pase si procentaj de 3 puncte (= APG * 2.0 + PPG * 1.0 + 3PT% * 10.0);
- **Forward** — evaluat dupa versatilitate (= PPG * 1.5 + RPG * 1.5 + FG% * 10.0);
- **Center** — evaluat dupa blocaje si recuperari (= RPG * 2.0 + BLK * 3.0 + PPG * 0.5);
- **TwoWayPlayer** — evaluat dupa rating defensiv (= PPG * 1.0 + RPG * 1.0 + APG * 1.0 + defensiveRating * 2.0);
  
Am introdus si un nou input: "compara", in care poti compara 2 jucatori, acestia confrantandu-se cap la cap prin statistici, precum PPG, APG, RPG, role score etc. Castigorul este bazat pe Impact Score.
La afisarea roster-ului si a clasamentului am facut prin biblioteca tabulate:
- **Echipa** - cand afisam jucatorii din echipe cei All-Stars sunt colorati cu verde, iar ceilalti in albastru, iar head-ul tabelului este galben.
- **Clasament** - aici am colorat numele echipelor care au iesit in top 3 pentru a iesi in evidenta.
Alte functii pe care le-am folosit in Stats pentru a genera **statistici agregate**:
- top jucatori;
- top All-Stars;
- media statisticilor din sezon;
- simulare meci intre doua echipe;
- echipe pe conferinta;

De asemenea, analizat si echipele, uramrind numarul de All-Stars din fiecare echipa, cand le expira contractele jucatorilor si daca o echipa este contender la titlu.
La final de predare am adaugat si logo-urile echipelor (momentan am pus in JSON scrisul si la echipele care pe parcursul aniilor si-au schimbat logo-ul le-am lasat logo-ul din sezonul actual, dar la tema 3 voi modifica cum trebuie).

Pentru ultima parte am mai adaugat cateva lucruri: am facut liga un singleton (NBALeague), ca sa incarc cele 13 sezoane o singura data, un sistem de observatori pentru simularea de playoff (un logger in consola plus colectori de meciuri si statistici) si o clasa sablon StatLeader<T> pentru topuri. 
Am bagat si o interfata grafica cu SFML, cu cateva comenzi noi:
-logo-urile echipelor dintr-un sezon, asezate intr-o grila cu numele sub fiecare ('logo');
-o animatie a finalei NBA, un fel de "fast forward" cu ceasul de la 48:00 la 00:00, in care scorul creste de la 0 pana la cel final ('campion');
-castigatoarea e cea data de simulare (dupa scorul de impact al echipelor), iar scorul propriu-zis e generat aleator dar reproductibil — pornesc generatorul din anul sezonului, asa ca aceeasi finala iese mereu cu acelasi scor;
-bracket-ul intreg de playoff, care se aranjeaza singur in functie de cate echipe are sezonul ('bracket');
-plus comenzi de tip playoff / simulare / allstars pentru rezultate si topuri in consola;


## Resurse

- dupa cum am zis si in partea de sus, am generat echipele cu toti jucatorii cu ajutorul inteligentei artificiale, deoarece ar fi luat mult sa implementez n jucatori pentru un anumit numar de echipe pentru 13 sezoane;
- ideea de bracket am luat-o de pe site-ul https://www.espn.com/nba/playoff-bracket, site oficial care contorizeaza chestii despre mai multe sporturi;
