
package mindmelt.action;

import mindmelt.engine.Engine;

public class Action {
    
    Engine engine;
    
    public boolean doAction(int param) {
        return true;
    }
    
    Action(Engine eng) {
        engine = eng;
    }
}
