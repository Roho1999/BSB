/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                       S T R I N G B U F F E R                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Die Klasse Stringbuffer stellt einen Puffer fuer die Sammlung von Zeichen */
/* zur Darstellung auf dem Bildschirm oder anderen Ausgabegeraeten bereit.   */
/* Die Ausgabe der Zeichen erfolgt, sobald der Puffer voll ist oder wenn     */
/* explizit die Methode flush() aufgerufen wird.                             */
/* Da Stringbuffer geraeteunabhaengig sein soll, ist flush() eine virtuelle  */
/* Methode, die von den abgeleiteten Klassen definiert werden muss.          */
/*****************************************************************************/

#ifndef __strbuf_include__
#define __strbuf_include__

class Stringbuffer
 {

public:
      Stringbuffer() = default;
      /**
       * @brief Diese Methode fügt das Zeichen c an das Ende der bereits gesammelten Zeichen an.
       *  Wenn die Pufferkapazität des Stringbuffer-Objektes nun erschöpft ist,
       *  müssen die gesammelten Zeichen durch Aufruf der Methode flush () verarbeitet werden    
       * 
       * @param c 
       */
      void put (char c);

      /**
       * @brief Mit dieser Methode sollen die gesammelten Zeichen verarbeitet und
       *  so Platz für neue Zeichen geschaffen werden. Die Implementierung der Methode soll jedoch erst in den abgeleiteten Klassen erfolgen.
       * 
       */
      virtual void flush()=0;
      char charbuffer[5];
      unsigned int position = 0;

private:
      Stringbuffer(const Stringbuffer &copy); // Verhindere Kopieren

      //hier könnten noch member Variablen hin: Ich denke, ungefähr so wie hier beschrieben
      //"Zur Pufferung der Zeichen eignet sich ein fest dimensioniertes Feld,
      // auf das die abgeleiteten Klassen zugreifen können müssen. Auch die Anzahl der Zeichen
      // oder das zuletzt beschriebene Feldelement sollte in den spezialisierten flush()-Methoden erkennbar sein."
 };

#endif
