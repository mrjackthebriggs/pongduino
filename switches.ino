
int8_t fourpoleselsw(int8_t id)
{
    if(id < 1 | id > 2)
        return 0;

    int16_t anVal = analogRead(id + 2);

    if(anVal < 250)
        return 1;
    else if(anVal < 500)
        return 2;
    else if(anVal < 750)
        return 3;
    else if(anVal < 1024)
        return 4;
    else
        return 0;
}

int8_t dipsw()
{
    int8_t total{};
    
    if(digitalRead(12))
        total += 1;
    
    if(digitalRead(2))
        total += 2;

    if(digitalRead(3))
        total += 3;
    
    if(digitalRead(4))
        total += 4;
    
    return total;
}
    