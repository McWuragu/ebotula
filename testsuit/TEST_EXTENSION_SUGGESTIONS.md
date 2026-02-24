# Vorschläge zur Erweiterung der Unit-Tests

## Ziel
Diese Vorschläge bauen auf den vorhandenen CUnit-Tests in `testsuit/utilities_test.c` und `testsuit/queue_test.c` auf und fokussieren auf Randfälle, Fehlerpfade und robuste Concurrency-Checks.

## Utilities (`trim`, `clearspace`, `StrToLower`, Channel-Parsing)

1. **`trim`: Tabulatoren zwischen Wörtern explizit prüfen**
   - Aktuell wird bereits gemischter Whitespace abgedeckt, aber kein expliziter Fall mit Tabs *zwischen* Token.
   - Beispiel: `"foo\t\tbar" -> "foobar"` (aktuelles Verhalten) oder gewünschtes Verhalten dokumentieren.

2. **`trim`: nur `\n`/`\r` ohne Text**
   - Beispiel: `"\n\r\n" -> ""`.
   - Schützt gegen Regressions bei reinem Zeilenumbruch-Input.

3. **`clearspace`: ungerade Anzahl Quotes ("broken quote")**
   - Beispiel: `"\"hello world"`.
   - Das Verhalten sollte per Test fixiert werden, da die Funktion intern einen `noclr`-Zustand toggelt.

4. **`StrToLower`: `NULL` als Input**
   - Expliziter Negativtest: `CU_ASSERT_PTR_NULL(StrToLower(NULL));`.

5. **`StrToLower`: sehr langer String**
   - Last-/Robustheitstest (z. B. 4k/8k Zeichen), um Buffer-Handling und Terminierung zu prüfen.

6. **`StrToChannelData`/`ChannelDataToStr`: optionale Felder leer**
   - Fälle ohne Topic/Greeting, nur Modes; ohne `+k` aber mit Keyword gesetzt usw.
   - Ziel: deterministische Serialisierung/Deserialisierung bei unvollständigen Daten.

## Queue (`pushQueue`, `popQueue`, `flushQueue`, Iterator)

1. **`flushQueue(NULL)` Fehlercode prüfen**
   - Der Code behandelt `NULL` explizit und liefert `QUEUE_NULL_POINTER_AS_IN_PARAMETER`.

2. **`getNextitrQueue` Iterationsreihenfolge komplett prüfen**
   - Mehrere Elemente pushen und mehrere Aufrufe von `getNextitrQueue`, um Reihenfolge und Iterator-Ende zu validieren.

3. **`popQueue` mit mehreren wartenden Threads**
   - 2+ Consumer blockieren lassen, danach mehrere Producer-Signale; prüfen, dass alle Threads sauber aufwachen.

4. **Stresstest: viele Push/Pop-Operationen**
   - Beispiel: 10.000 Pushes + 10.000 Pops in Schleife, danach `isemptyQueue == true`.

5. **Speicher-/Ressourcenstabilität mit Sanitizern**
   - Testlauf unter `ASAN/UBSAN` ergänzen, um Leaks und Use-after-free bei Queue-Operationen früh zu erkennen.

6. **`deleteQueue`-Nutzung klar absichern**
   - Kein API-Call auf gelöschter Queue, aber Test kann dokumentieren, dass `deleteQueue(NULL)` aktuell nicht unterstützt ist (falls so gewollt, alternativ defensiv implementieren + testen).

## Priorisierte Reihenfolge (Quick Wins)

1. `StrToLower(NULL)`
2. `flushQueue(NULL)`
3. `getNextitrQueue` über mehrere Elemente
4. `clearspace` mit unbalancierten Quotes
5. kleiner Concurrency-Test mit 2 wartenden `popQueue`-Threads
