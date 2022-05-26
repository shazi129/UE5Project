import * as UE from 'ue';

class TS_TestTickActor extends UE.Actor
{
    ReceiveTick(DeltaSeconds: number) : void
    {
        console.log("TS_TestTickActor.ReceiveTick=======    ========= " + DeltaSeconds);
    }
}

export default TS_TestTickActor;