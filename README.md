# Simulator NBA

În acest proiect am făcut un simulator de sezoane, în opinia mea, interesante, din NBA. Aplicația permite selectarea unui sezon din cele 13 la dispoziție (ex: 1965-66, 1999-00, 2007-08, 2022-23 etc.).

Fiecare sezon are echipe cu lotul întreg și cu stats-urile jucătorilor adevărate (aceste informații au fost generate de inteligența artificială, deoarece mi-ar fi luat foarte mult timp să scriu câte 12 jucători per echipă pentru 9, 17, 23, 27, 29 sau 30 de echipe, depinde de sezon). Pentru a stabili dacă un jucător este All-Star sau Role Player am făcut o medie a statisticilor, deoarece acest lucru este diferit de la sezon la sezon. (ex: în anul 1965-66 un jucător All-Star ar fi avut stats-uri mai slabe decât unul din sezonul 2022-23. De aceea am pus condițiile: >= 18 PPG || >= 14 PPG && >= 7 APG || >= 14 PPG && >= 8 RPG).

- **PPG** = Points Per Game (puncte per meci);
- **APG** = Assists Per Game (pase per meci);
- **RPB** = Rebounds Per Game (recuperări per meci);
  
Funcționalități principale:
- Explorarea echipelor și jucătorilor din fiecare sezon;
- Calculul statisticilor individuale (PPG, APG, RPG);
- Identificarea celui mai bun jucător dintr-un sezon = impact score (am calculat prin formula:
  PPG + APG * 0.75 + RPG * 0.5);
- Determinarea favoritei la titlu pe baza scorului de impact (am folosit aceeași formulă ca la cel mai bun jucător, iar dacă un jucător este All-Star conform unor criterii de puncte, pase sau recuperări, rezultatul este înmulțit cu 1.5. Favorita la titlu se află prin suma tuturor scorurilor de impact ale jucătorilor din fiecare echipă);

Am extins simulatorul cu o ierarhie de moșteniri bazată pe pozițiile jucătorilor. Clasa abstractă `Player` are 4 clase derivate: `Guard`, `Forward`, `Center` și `TwoWayPlayer`, fiecare cu propria formulă de evaluare prin `calculateRoleScore()`:

- **Guard** — evaluat după pase și procentaj de 3 puncte (= APG * 2.0 + PPG * 1.0 + 3PT% * 10.0);
- **Forward** — evaluat după versatilitate (= PPG * 1.5 + RPG * 1.5 + FG% * 10.0);
- **Center** — evaluat după blocaje și recuperări (= RPG * 2.0 + BLK * 3.0 + PPG * 0.5);
- **TwoWayPlayer** — evaluat după rating defensiv (= PPG * 1.0 + RPG * 1.0 + APG * 1.0 + defensiveRating * 2.0);
  
Am introdus și un nou input: "compară", în care poți compara 2 jucători, aceștia confruntându-se cap la cap prin statistici, precum PPG, APG, RPG, role score etc. Câștigătorul este bazat pe Impact Score.
La afișarea roster-ului și a clasamentului am făcut prin biblioteca tabulate:
- **Echipa** - când afișăm jucătorii din echipe cei All-Stars sunt colorați cu verde, iar ceilalți în albastru, iar head-ul tabelului este galben.
- **Clasament** - aici am colorat numele echipelor care au ieșit în top 3 pentru a ieși în evidență.
Alte funcții pe care le-am folosit în Stats pentru a genera **statistici agregate**:
- top jucatori;
- top All-Stars;
- media statisticilor din sezon;
- simulare meci între două echipe;
- echipe pe conferință;

De asemenea, am analizat și echipele, urmărind numărul de All-Stars din fiecare echipă, când le expiră contractele jucătorilor și dacă o echipă este contender la titlu.
La final de predare am adăugat și logo-urile echipelor (pentru echipele de la începutul ligii, adică din primul sezon existent, 1965-66, am pus aceleași logo-uri ca cele de la echipele din prezent, deoarece nu am găsit toate logo-urile care trebuiau și erau multe poze de salvat, echipele schimbându-și logo-ul o dată la câțiva ani).

Pentru ultima parte am mai adăugat câteva lucruri: am făcut liga un singleton (NBALeague), ca să încarc cele 13 sezoane o singură dată, un sistem de observatori pentru simularea de playoff (un logger în consolă plus colectori de meciuri și statistici) și o clasă șablon `StatLeader<T>` pentru topuri.

Am implementat și o interfață grafică cu SFML, cu câteva comenzi noi:
- logo-urile echipelor dintr-un sezon, așezate într-o grilă cu numele sub fiecare ('logo');
- o animație a finalei NBA, un fel de "fast forward" cu ceasul de la 48:00 la 00:00, în care scorul crește de la 0 până la cel final ('campion');
- câștigătoarea e cea dată de simulare (după scorul de impact al echipelor), iar scorul propriu-zis e generat aleator, dar reproductibil — pornesc generatorul din anul sezonului, așa că aceeași finală iese mereu cu același scor;
- bracket-ul întreg de playoff, care se aranjează singur în funcție de câte echipe are sezonul ('bracket');
- plus comenzi de tip 'playoff' / 'simulare' / 'allstars' pentru rezultate și topuri în consolă;


## Resurse

- după cum am zis și în partea de sus, am generat echipele cu toți jucătorii cu ajutorul inteligenței artificiale, deoarece ar fi luat mult să implementez n jucători pentru un anumit număr de echipe pentru 13 sezoane;
- ideea de bracket am luat-o de pe site-ul https://www.espn.com/nba/playoff-bracket, site oficial care contorizează chestii despre mai multe sporturi;
- majoritatea logo-urilor le-am luat de pe acest site: https://www.pngwing.com/en/search?q=NBA, iar pe celelalte le-am luat de pe acest site: https://en.wikipedia.org/wiki/Category:NBA_logos;
