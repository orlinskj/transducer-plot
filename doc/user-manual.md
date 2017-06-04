# Podręcznik użytkownika

## Okno główne

Po uruchomieniu programu użytkownik ma do dyspozycji okno główne programu, które zawiera 3 główne obszary, przyciski do zarządzania zasobami oraz pasek menu (ukryty na zrzucie ekranu poniżej).

![Okno główne - obszary](https://raw.githubusercontent.com/orlinskj/transducer-plot/master/doc/img/main-window-areas.png "Obszary okna głównego")

### Pasek menu

#### Zrzut ekranu

Aby zrobić zrzut ekranu z aktualnie wyświetlanego wykresu, należy z paska menu wybrać `Wykres -> Zrzut ekranu`. Następnie program wyświetli panel zawierający dostępne opcje.

![Panel zrzutu ekranu](https://raw.githubusercontent.com/orlinskj/transducer-plot/master/doc/img/screenshot-panel.png "Panel zrzutu ekranu")

Górne pole tekstowe służy do wprowadzania nazwy pliku. Klikając w przycisk `…` obok pola tekstowego użytkownik może otworzyć okno do wybrania pliku, które znacznie ułatwia wybieranie żądanej lokalizacji.

Dolne pole tekstowe służy do wprowadzania żądanych rozmiarów dla zrzutu ekranu. Użytkownik może wybrać istniejący rozmiar z listy, lub wprowadzić swój, zatwierdzając klawiszem `Enter`.

Przyciski po prawo służą do zapisania obrazu na dysku (przycisk z ikoną dyskietki) lub zamknięcia panelu zrzutu ekranu (przycisk z ikoną krzyżyka).

#### Przetwornik

Opcje menu dostępne pod kategorią `Przetwornik` otwierają okno przetwornika na żądanej zakładce.

#### Pomoc

Z poziomu kategorii `Pomoc` możemy wyświetlić okno zaierające podstawowe informacje o programie `Pomoc -> O programie` oraz uzyskać pomoc na temat programu. Kliknięcie przycisku `Pomoc -> Pomoc` otwiera tekst który aktualnie czytasz.

### Podgląd wykresu

Podgląd wykresu jest obszarem, na którym rysowane są wykresy. Zajmuje on większą część okna głównego.

**Przybliżanie i oddalanie**

Przybliżenia lub oddalenia narysowanego wykresu można dokonać przytrzymując lewy klawisz `Ctrl` oraz klikają na wykresie `Lewy przycisk myszy` jednocześnie przeciągając kursor w osi pionowej. Ruch do góry przybliża wykres, ruch do dołu oddala.

Możliwe jest również przybliżenie lub oddalenie wyłączenie w osi poziomej. W celu takiego przybliżenia należy przytrzymać klawisz `x` oraz kliknąć na wykresie `Lewy przycisk myszy` oraz jednocześnie przeciągnąć kursor myszy w osi pionowej.

**Podgląd wartości (miotła)**

Przesuwanie kursora myszy przesuwa również Miotłę. Miotła służy do szybkiego podglądu wartości na krzywych, z którymi się przecina. Górna wartość liczbowa wyświetlana na miotle odpowiada wartości ze zbioru dziedziny na której aktualnie się ona znajduje. Wartości znajdujące się niżej opisują wartości na poszczególnych krzywych.

![Miotła](https://raw.githubusercontent.com/orlinskj/transducer-plot/master/doc/img/broom.png "Miotła")

Miotłę można również "przyczepić" w żądanym miejscu wykresu klikając `Lewy przycisk myszy` w żądanym miejscu. Kolejne kliknięcie tego same przycisku w dowolnym miejscu na wykresie spowoduje "odklejenie" Miotły.

**Zmiana rodzaju osi (liniowa/logarytmiczna)**

Klikając `Prawy przycisk myszy` na podglądzie wykresu, uzyskamy dostęp do menu kontekstowego, z poziomu którego możemy zmienić rodzaj każdej z osi pionowych wykresu (liniowa/logarytmiczna).

**Oznaczenia na kole admitancji**

Po utworzeniu wykresu koła admitancji (`Re->Im` lub `Im->Re`) program automatycznie dodaje do tego wykresu stosowne oznaczenia.

![Oznaczenia na kole admitancji](https://raw.githubusercontent.com/orlinskj/transducer-plot/master/doc/img/admitance-ring.png "Oznaczenia na kole admitancji")

Na na wykresie koła admitancji rysowane są:
- środek tego koła (przecinające się szare linie)
- półprosta wychodząca z początku układu współrzędnych i przechodząca przez środek koła (różowa linia)
- punkt przecięcia półprostej z kołem wraz z oznaczoną częstotliwością w tym punkcie

### Lista wykresów

Lista wykresów prezentuje wszystkie wykresy utworzone w programie. Lista znajduje się z prawej strony, w górej części okna głównego. Każdy z wykresów może posiadać dowolną liczbę krzywych, pod warunkiem, że wszystkie krzywe przedstawiają funkcje określone na tej samej dziedzinie.

**Pokazanie wykresu**

Każdy z wykresów można pokazać na podglądzie klikając dwa razy `Lewy przycisk myszy` na wybranym wykresie. Aktualnie wyświetlany wykres jest oznaczony na liście wykresów za pomocą ikony oka.

![Oznaczenie wyświetlanego wykresu](https://raw.githubusercontent.com/orlinskj/transducer-plot/master/doc/img/plot-showed.png "Oznaczenie aktualnie wyświetlanego wykresu")

### Przyciski

Przyciski służą do zarządania zasobami w programie, za ich pomocą możemy dodawać i usuwać przetworniki, wykresy, i funkcje.

**Dodawanie/usuwanie przetwornika**

W celu dodania przetwornika należy kliknąć przycisk ![Przycisk dodaj przetwornik](https://raw.githubusercontent.com/orlinskj/transducer-plot/master/doc/img/add-transducer-button.png "Przycisk ładujący nowy przetwornik"), następnie program wyświetli pole wyboru pliku, po czym należy wybrać żądany plik przetwornika.

W celu usunięcia przetwornika należy najechać kursorem myszy na dany przetwornik. Program wyświetli wtedy przycisk z ikoną krzyża służący do usuwania. Usunięcie przetwornika usuwa także wszystkie krzywe, które były stworzone z jego udziałem.

**Dodawanie/usuwanie wykresu**

W celu dodania wykresu należy kliknąć przycisk ![Przycisk dodaj przetwornik](https://raw.githubusercontent.com/orlinskj/transducer-plot/master/doc/img/add-plot-button.png "Przycisk tworzący nowy wykres"), po naciśnięciu którego system doda nowy wykres do listy.

W celu usunięcia wykresu należy najechać kursorem myszy na dany wykres na liście. Program wyświetli wtedy przycisk z ikoną krzyża służący do usuwania. Kliknięcie w przycisk usuwa wykres wraz ze wszystkimi funkcjami.

**Dodawanie/usuwanie krzywych**

W celu dodania funkcji, należy kliknąć przycisk ![Przycisk dodaj funkcję](https://raw.githubusercontent.com/orlinskj/transducer-plot/master/doc/img/add-function-button.png "Przycisk tworzący nową funkcję"), po naciśnięciu którego program wyświetli panel wyboru zbiorów dla nowej krzywej.

![Panel dodawania nowej krzywej](https://raw.githubusercontent.com/orlinskj/transducer-plot/master/doc/img/function-add.png "Panel dodawania nowej krzywej")

Z poziomu panelu dostępna jest lista wszystkich zbiorów danych dostępnych dla aktualnie wybranego przetwornika. Przetwornik wybierany jest z listy przetworników znajdującej się po prawo (w dolnej części).

Klikając `Lewy przycisk myszy` na żądane zbiory danych należy wybrać dziedzinę i przeciwdziedzinę funkcji. Po wybraniu elementy z listy przybierają różowy kolor oraz otrzymują stosowne oznaczenie (`dziedzina`/`przeciwdziedzina`). Po wybraniu zarówno dziedziny jak i przeciwdziedziny, w lewej części panelu dostępny jest podgląd krzywej wykreślonej przez wybrane zbiory.

Ostatecznie należy upewnić się, że został wybrany odpowiedni wykres, do którego chcemy dodać nową krzywą. Wykres wybieramy klikając `Lewy przycisk myszy` na odpowiedni element z listy wykresów znajdującej się po prawo w górej częsci.

Po wybraniu wszystkich opcji należy nacisnąć przycisk `Dodaj`, po naciśnięciu którego program doda nową funkcję do wykresu i pokaże go na podglądzie wykresu.

W celu usunięcia krzywej funkcji należy najechać kursorem myszy na daną funckję na liście. Program wyświetli wtedy przycisk z ikoną krzyża służący do usuwania. Kliknięcie w przycisk usuwa funkcję z wykresu.

### Lista przetworników

Lista przetworników prezentuje wszystkie załadowane do programu pliki przetworników. List znajduje się po prawej, w dolnej części okna głównego. Pod nazwą przetwornika wyświetla się ścieżka bezwzględna do jego pliku.

## Okno przetwornika

![Okno przetwornika](https://raw.githubusercontent.com/orlinskj/transducer-plot/master/doc/img/transducer-dialog.png "Okno przetwornika")

Okno przetwornika służy do podglądu danych przetwornika oraz eksportu. W górnej jego części znajduje się pole wyboru, w którym użytkownik może wybrać, dane którego przetwornika chce wyświetlać. Poniżej znajdują się 3 zakładki udostępniające różne informacje i akcje.

### Dane tabelaryczne

![Zakładka dane tabelaryczne](https://raw.githubusercontent.com/orlinskj/transducer-plot/master/doc/img/tab-tabular-data.png "Zakładka dane tabelaryczne")

Zakładka `Dane tabelaryczne` wyświetla wszystkie zbiory danych dostępne dla przetwornika w postaci tabeli.

### Modele zastępcze

![Zakładka modele zastępcze](https://raw.githubusercontent.com/orlinskj/transducer-plot/master/doc/img/tab-models.png "Zakładka modele zastępcze")

Zakładka `Modele zastępcze` służy do podglądu wyliczonych parametrów modelu BVD przetwornika. W prawej części zakładki wyświetlane są wartości parametrów dla wybranego przetwornika.

**Zmiana typu modelu zastępczego**

Użytkownik może wybrać rodzaj modelu (`Szeregowy`/`Równoległy`) z poziomu pola wyboru z górnej lewej części zakładki.

**Wprowadzanie ustalonej pojemności wejściowej**

Domyślnie program wylicza pojemność wejściową `Cp` przetwornika na podstawie, dostarczonych danych pomiarowych dostępnych dla najniższej odczytanej częstotliwości.

Aby zadać zmierzoną wartość pojemności wejściowej przetwornika, należy zaznaczyć pole `Ustalona pojemność wejściowa`, a następnie wpisać w polu po prawo zadaną wartość tego parametru.

![Pole ustalona pojemność wejściowa](https://raw.githubusercontent.com/orlinskj/transducer-plot/master/doc/img/tab-models-set-cop.png "Pole ustalona pojemność wejściowa")

Pole przyjmuje wartości w notacji dziesiętnej i naukowej, uwzględnia także przedrostki jednostek, w związku z czy wszystkie wartości z następujących są poprawne i wyrażają tą samą wartość: `10nF`, `1e-8`, `0.00000001`.

### Eksport

![Zakładka eksportu](https://raw.githubusercontent.com/orlinskj/transducer-plot/master/doc/img/tab-export.png "Zakładka eksportu")

Zakładka `Eksport` służy do generowania dokumentów opisujących przetwornik.

**Zmiana formatu wyjściowego**

Użytkownik ma do wyboru dwa formaty wyjściowe: PDF (Portable Document Format) oraz CSV (Comma-Separated Values) dostępne do wyboru w polu wyboru w górnym prawym rogu zakładki.

Dokument w formacie PDF może zawierać wyliczone wartości parametrów modelu zastępczego oraz obrazy przedstawiające wszystkie krzywe w dziedzinie częstotliwości dostępne dla przetwornika.

Dokument w formacie CSV może zawierać jedynie dane tabelaryczne przetwornika. Format ten jest użyteczny w przypadku konieczności załadowania danych do arkusza kalkulacyjnego - większość pakietów biurowych obsługuje pliki wejściowe w formacie CSV.

**Eksport do pliku**

W celu wyeksportowania informacji na temat przetwornika należy wybrać żądany format eksportu, następnie wybrać żądane opcje eksportu. Nazwa pliku do eksportu jest wprowadzana w polu tekstowym na dole. W celu wygodnego wybrania żądanej lokalizacji za pomocą okna wyboru pliku, należy kliknąć przycisk `…` znajdujący się w prawej części pola tekstowego.
