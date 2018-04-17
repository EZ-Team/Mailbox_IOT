   const int LASER = D5;
// Fonction setup(), appelée au démarrage de la carte Arduino
void setup() {

  digitalWrite(LASER, HIGH);   // turn the Laser on (HIGH is the voltage level)

    // initialize digital pin LED_BUILTIN as an output.
  pinMode(LASER, OUTPUT);

  // Initialise la communication avec le PC
  Serial.begin(9600);
}

// Fonction loop(), appelée continuellement en boucle tant que la carte Arduino est alimentée
void loop() {
  
  // Mesure la tension sur la broche A0
  int valeur = analogRead(A0);
  
  // Envoi la mesure au PC pour affichage et attends 250ms
  Serial.println(valeur);
  delay(100);
}
