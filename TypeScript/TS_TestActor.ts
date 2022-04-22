import * as UE from "ue"

class TS_TestActor extends UE.Actor
{
    // override ReceiveBeginPlay() : void
    // {
    //     console.log("ReceiveBeginPlay =================================");
    // }

    // override ReceiveDestroyed() : void
    // {
    //     console.log("ReceiveDestroyed =================================");
    // }

    TestFunc(): void
    {
        console.log("TestFunc===========================");
    }
}

export default TS_TestActor;