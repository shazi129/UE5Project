import * as UE from 'ue';

class TS_TestActor extends UE.Actor
{
    ReceiveTick(DeltaSeconds: number) : void
    {
        console.log("================ " + DeltaSeconds);
    }
}

export default TS_TestActor;