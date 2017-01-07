package mindmelt.action;

import mindmelt.engine.Engine;
import mindmelt.objects.Obj;


public class ZoneAdjacentAction extends Action {
    
    public ZoneAdjacentAction(Engine engine) {
        super(engine);
    }
    
    public boolean doAction(int zone) {
        System.out.println("Zone: zone="+zone);
        int zx = zone % 3 - 1;
        int zy = zone / 3 - 1;
        System.out.println("Adjacent x="+zx+" y="+zy);
        
        Obj player = engine.getObjects().getPlayer();
        int px = player.getX();
        int py = player.getY();
        int pz = player.getZ();
        int d = player.getDirection();

        int a[] = {zx,-zy,-zx,zy};
        int b[] = {zy,zx,-zy,-zx};
        
        int x = px+a[d];
        int y = py+b[d];
        
        //pickup/drop
        Obj ob = engine.getTopObject(x, y, pz);
        if (ob!=null && !ob.isPlayer()) {
            engine.moveObjToObj(ob, player);
        } else {
            engine.activateTile(x,y,pz);
        }
        return true;
    }
}
