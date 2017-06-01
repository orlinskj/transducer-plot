# Podręcznik użytkownika

## Okno główne

Po uruchomieniu programu użytkownik ma do dyspozycji okno główne programu, zawiera 5 głównych obszarów oraz pasek menu.

<img src="okno-główne-obszary">

### Pasek menu

#### Zrzut ekranu

Aby zrobić zrzut ekranu aktualnie wyświetlanego wykresu, należy z paska menu wybrać `Wykres->Zrzut ekranu`. Następnie program wyświetli panel zawierający dostępne opcje.

<img src="zrzut-ekranu-panel-oznaczenia">

Górne pole tekstowe [1] służy do wprowadzania nazwy pliku. Klikając w przycisk `…` [2] obok pola tekstowego użytkownik może otworzyć okno do wybrania pliku, które znacznie ułatwia wybieranie żądanej lokalizacji.

Dolne pole tekstowe [3] służy do wprowadzania żądanych rozmiarów dla zrzutu ekranu. Użytkownik może wybrać istniejący rozmiar z listy, lub wprowadzić swój, zatwierdzając klawiszem `enter`.

Przyciski po prawo [4] służą do zapisania obrazu na dysku (przycisk z ikoną dyskietki) lub zamknięcia panelu zrzutu ekranu (przycisk z ikoną krzyżyka).

#### Przetwornik

Opcje menu dostępne pod kategorią `Przetwornik` otwierają okno przetwornika [LINK] na żądanej zakładce.

#### Pomoc

Z poziomu kategorii `Pomoc` możemy wyświetlić okno zaierające podstawowe informacje o programie `Pomoc->O programie` oraz uzyskać pomoc na temat programu. Kliknięcie przycisku `Pomoc->Pomoc (online)` otwiera z poziomu przeglądarki internetowej plik, który aktualnie czytasz.

### Podgląd wykresu

Podgląd wykresu jest obszarem, na którym rysowane są wykresy.

**Przybliżanie i oddalanie**

Przybliżenia lub oddalenia narysowanego wykresu można dokonać przytrzymując lewy klawisz `Ctrl` oraz klikają na wykresie `lewy przycisk myszy` jednocześnie przeciągając kursor w osi pionowej. Ruch do góry przybliża wykres, ruch do dołu oddala.

Możliwe jest również przybliżenie lub oddalenie wyłączenie w osi poziomej. W celu takiego przybliżenia należy przytrzymać klawisz `x` oraz kliknąć na wykresie `lewy przycisk myszy` oraz jednocześnie przeciągnąć kursor myszy w osi pionowej.

**Podgląd wartości (miotła)**

Przesuwanie kursora myszy przesuwa również Miotłę. Miotła służy do szybkiego podglądu wartości na krzywych, z którymi się przecina. Górna wartość liczbowa wyświetlana na miotle odpowiada wartości ze zbioru dziedziny na której aktualnie się ona znajduje. Wartości znajdujące się niżej opisują wartości na poszczególnych krzywych.

<img src="miotła">

Miotłę można również "przyczepić" w żądanym miejscu wykresu klikając `lewy przycisk myszy` w żądanym miejscu. Kolejne kliknięcie tego same przycisku w dowolnym miejscu na wykresie spowoduje "odklejenie" Miotły.

**Zmiana rodzaju osi (liniowa/logarytmiczna)**

Klikając `prawy przycisk myszy` na podglądzie wykresu, uzyskamy dostęp do menu kontekstowego z poziomu, którego możemy zmienić rodzaj każdej z osi pionowych wykresu liniowy/logarytmiczny.

### Lista wykresów

Lista wykresów prezentuje wszystkie wykresy utworzone w programie. Każdy z wykresów może posiadać dowolną liczbę krzywych, pod warunkiem, że wszystkie krzywe przedstawiają funkcje określone na tej samej dziedzinie.

**Rozwinięcie wykresu (podląd funkcji)**

Każdy z wykresów można rozwinąć w celu pokazania listy wszystkich funkcji, które zawiera. Aby rozwinąć lub rozwinąć wykres, należy kliknąć w przełącznik w postaci trójkąta [1] znajdujący się w lewej części pola.

<img src="wykres-przełącznik">

### Przyciski

Przyciski służą do zarządania zasobami w programie, za ich pomocą możemy dodawać i usuwać przetworniki, wykresy, i funkcje.

**Dodawanie/usuwanie przetwornika**

W celu dodania przetwornika należy kliknąć przycisk `t+`, następnie program wyświetli pole wyboru pliku, po czym należy wybrać żądany plik przetwornika.

**Dodawanie/usuwanie wykresu**

W celu dodania wykresu należy kliknąć przycisk `p+`, po naciśnięciu którego system doda nowy wykres do listy.

**Dodawanie/usuwanie krzywych**

W celu dodania funkcji, należy zaznaczyć wykres na liście, do którego chcemy dodać funkcję. Zaznaczony wykres zostanie podświetlony na inny kolor. Następnie należy kliknąć przycisk `f+`, po naciśnięciu którego program wyświetli nowe okno z wyborem przetwornika i zbiorów dla nowej krzywej.

<img src="nowa-krzywa">

W górnej części należy wybrać przetwornik, dla którego chcemy utworzyć krzywą. Z pól na dole należy wybrać - klikając na odpowiednie pola - zbiory tworzące dziedzinę i przeciwdziedzinę krzywej. Po kliknięciu przycisku `Dodaj` program zamknie okno wyboru nowej krzywej i doda ją do wybranego wcześniej wykresu.

### Lista przetworników

Lista przetworników prezentuje wszystkie załadowane do programu pliki przetworników. Pod nazwą przetwornika wyświetla się ścieżka bezwzględna do jego pliku.

## Okno przetwornika

<img src="okno-przetwornik">

Okno przetwornika służy do podglądu przetwornika. W górnej jego części znajduje się pole wyboru [1], w którym użytkownik może wybrać, dane którego przetwornika chce wyświetlać. Poniżej znajdują się 3 zakładki [2] udostępniające różne informacje i akcje.

### Dane tabelaryczne

<img src="dane-tabelaryczne">

Zakładka `Dane tabelaryczne` wyświetla wszystkie zbiory danych dostępne dla przetwornika w postaci tabeli.

### Modele zastępcze

<img src="modele-zastępcze">

Zakładka `Modele zastępcze` służy do podglądu wyliczonych parametrów modelu BVD przetwornika. W prawej części zakładki [2] wyświetlane są wartości parametrów dla wybranego przetwornika.

**Zmiana typu modelu zastępczego**

Użytkownik może wybrać rodzaj modelu (`Szeregowy`/`Równoległy`) z poziomu pola wyboru z górnej lewej części zakładki [1].

**Wprowadzanie ustalonej pojemności wejściowej**

Domyślnie program wylicza pojemność wejściową `Cp` przetwornika na podstawie, dostarczonych danych pomiarowych dostępnych dla najniższej odczytanej częstotliwości.

Aby zadać zmierzoną wartość pojemności wejściowej przetwornika, należy zaznaczyć pole `Ustalona pojemność wejściowa` [3], a następnie wpisać w polu po prawo [4] zadaną wartość tego parametru. Pole przyjmuje wartości w notacji dziesiętnej i naukowej, uwzględnia także przedrostki jednostek, w związku z czy wszystkie wartości z następujących są poprawne i wyrażają tą samą wartość: `10nF`, `1e-8`, `0.00000001`.

### Eksport

<img src="eksport">

Zakładka `Eksport` służy do generowania dokumentów opisujących przetwornik.

**Zmiana formatu wyjściowego**

Użytkownik ma do wyboru dwa formaty wyjściowe: PDF (Portable Document Format) oraz CSV (Comma-Separated Values) dostępne do wyboru w polu wyboru [1].

Dokument w formacie PDF może zawierać wyliczone wartości parametrów modelu zastępczego oraz obrazy przedstawiające wszystkie krzywe w dziedzinie częstotliwości dostępne dla przetwornika.

Dokument w formacie CSV może zawierać jedynie dane tabelaryczne przetwornika. Format ten jest użyteczny w przypadku konieczności załadowania danych do arkusza kalkulacyjnego - większość pakietów biurowych obsługuje pliki wejściowe w formacie CSV.

**Eksport do pliku**

W celu wyeksportowania informacji na temat przetwornika należy wybrać żądany format eksportu, następnie wybrać żądane opcje eksportu [2]. Nazwa pliku do eksporu jest wprowadzana w polu tekstowym na dole [3]. W celu wygodnego wybrania żądanej lokalizacji za pomocą okna wyboru pliku, należy kliknąć przycisk `…` znajdujący się w prawej części pola tekstowego [4].
