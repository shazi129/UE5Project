"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
class TS_TestActor extends UE.Actor {
    ReceiveTick(DeltaSeconds) {
        console.log("================ " + DeltaSeconds);
    }
}
exports.default = TS_TestActor;
//# sourceMappingURL=TS_TestActor.js.map