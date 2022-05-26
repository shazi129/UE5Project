"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
class TS_TestTickActor extends UE.Actor {
    ReceiveTick(DeltaSeconds) {
        console.log("TS_TestTickActor.ReceiveTick=======    ========= " + DeltaSeconds);
    }
}
exports.default = TS_TestTickActor;
//# sourceMappingURL=TS_TestTickActor.js.map