const unsigned int TRIG_PIN=13;
const unsigned int ECHO_PIN=12;
const  unsigned int BAUD_RATE=9600;
int dists[5] = {-1,-2,-3,-4,-5};
int curr = 0;
void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(BAUD_RATE);
}

bool distcheck(int arr[]){
  if (arr[4] < 0){
    return false;
  }
  int high = 0; int low = 0;
  for (int i = 0; i < 4; i++){
    if ((arr[i+1] - arr[i]) > 0){
      high += 1;
    }
    else if (arr[i] < 20){
      low += 1;
    }
    else{
      high += 1;
    }
  }
  if (low >= high){
    return true;
  }
  else{
    return false;
  }
}

void loop()  {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN,  HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  

 long duration= pulseIn(ECHO_PIN, HIGH);
 int distance= duration/29/2;
  if(duration==0){
   Serial.println("Warning: no pulse from sensor");
   }  
  else{
    Serial.println(distance);
    if (curr >= 5){
      curr = 0;
    }
    dists[curr] = distance;
    curr += 1;
  }
  if (distcheck(dists)){
    Serial.println("hello");
  }
 delay(100);

 }