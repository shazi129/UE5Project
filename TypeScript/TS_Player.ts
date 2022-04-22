import * as UE from 'ue'

class TS_Player extends UE.Character 
{
    @UE.ufunction.ufunction(
        UE.ufunction.BlueprintCallable
    )
    public OnInput(data:string): void
    {
        console.log("dddddddd");
    }
}

export default TS_Player;