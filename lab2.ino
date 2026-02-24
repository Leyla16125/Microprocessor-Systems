uint8_t A,B,R,S;

void setup(){ 
  Serial.begin(9600); 
}

void loop(){
  if(!Serial.available()) return;
  A=Serial.parseInt(); 
  B=Serial.parseInt();
  while(Serial.available()) Serial.read();

  asm volatile(
    "mov r16,%[a]\n"
    "mov r17,%[b]\n"
    "sub r16,r17\n"
    "in  r19,__SREG__\n" 
    "mov %[r],r16\n"
    "mov %[s],r19\n"
    : [r]"=r"(R), [s]"=r"(S)
    : [a]"r"(A), [b]"r"(B)
    : "r16","r17","r19"
  );

  Serial.print(" A=");Serial.print(A);
  Serial.print(" B=");Serial.print(B);
  Serial.print(" R=");Serial.print(R);
  Serial.print(" Z=");Serial.print((S>>1)&1);
  Serial.print(" C=");Serial.println(S&1);

}