
// Added by LTH, Lars Breum Hansen, M.Sc, March 2025.

package simu5g.apps.mec.MecApps.PredictMecApp;
import simu5g.apps.mec.MecApps.MecAppBase;

simple PredictMecApp extends MecAppBase
{
    parameters:
        @class(PredictMecApp);
        mecAppId = default(0); // it will be override by getId() in initialize
        mecAppIndex = default(0);

        double lambda @unit(s) = default(10ms);

        //resource required info
        requiredRam = default(10MB);
        requiredDisk = default(100MB);
        requiredCpu = default(15000);
        
        double startPredictTime @unit(s) = default(5s); // Define delay parameter with seconds unit

        @display("i=block/app");
}
