void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  cli();

  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS12) | (1 << CS10);             // Set prescalar of timer 1 to 1024  
  TIMSK1 = (1 << OCIE1A);                       // Enable compare match A on timer 1
  OCR1A = 15624;

  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 255;// = (16*10^6) / (2000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS02) | (0 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);

  sei();  // Enable back the interrupts
}

void loop() {}

ISR(TIMER1_COMPA_vect) {
  //TCNT1 = 0;  // Reset the counter
  //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

ISR(TIMER0_COMPA_vect) {
  TCNT0 = 0;  // Reset the counter
  //Serial.println("test");
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
