# IDS
sudo apt-get install libsqlite3-dev

sudo apt-get install libxml2 libxml2-dev


Im Ordner libsodium-stable:

./configure

make && make check

sudo make install

sudo ln -s /usr/local/lib/libsodium.so.23 /usr/lib/libsodium.so.23

Aufruf: ./IDS [OPTION]... [PFAD]...

Scannen des angegebenen PFADes. Standard ist das root '/' Verzeichnis.

Erforderliche Argumente für lange Optionen sind auch für kurze erforderlich.

        -c, --check             Führe das Programm für ein Verzeichnis aus. Standard ist das root '/' Verzeichnis.

        -h, --help              Gibt diese Anleitung aus.

        -i, --init              Führt die Initialanweisungen für den ersten Start des Programms aus.

        --show_changed          Zeigt die geänderten Dateien an. Wenn die --verbose Option verwendet wird, wird diese Option ignoriert.

        --show_exclude          Zeigt die zu ignorierenden Pfade an. Pfade sind in der rules.xml Datei definiert.

        --show_new              Zeigt die neuen Dateien an. Wenn die --verbose oder --show_changed Option verwendet wird, wird diese Optionignoriert.

        -u, --update            Aktualisiert die Checksumme für geänderte Dateien. Diese Option wird zusammen mit --check verwendet.

        -v, --verbose           Zeigt ausführliche Informationen an.


Verfügbare Hashalgorithmen:

        SHA256

        SHA512

        libsodium Benutzt BLAKE2b

LICENSE GNU General Public License v3.0