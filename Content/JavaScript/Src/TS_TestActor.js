"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
class TS_TestActor extends UE.Actor {
    // override ReceiveBeginPlay() : void
    // {
    //     console.log("ReceiveBeginPlay =================================");
    // }
    // override ReceiveDestroyed() : void
    // {
    //     console.log("ReceiveDestroyed =================================");
    // }
    TestFunc() {
        console.log("TestFunc===========================");
    }
}
exports.default = TS_TestActor;
//# sourceMappingURL=TS_TestActor.js.map