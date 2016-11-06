/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mindmelt;

import java.util.Random;
import org.newdawn.slick.state.*;
import org.newdawn.slick.font.*;
import org.newdawn.slick.*;
import java.awt.Font;
import org.lwjgl.input.Mouse;
import org.newdawn.slick.state.transition.FadeInTransition;
import org.newdawn.slick.state.transition.FadeOutTransition;
        
public class MainMenu extends BasicGameState {

    private Image start;
    private Random rand;
    private TrueTypeFont ttf;
    private Sound scream;

    @Override
    public int getID() {
        return Mindmelt.mainMenu;
    }

    @Override
    public void init(GameContainer gc, StateBasedGame state) throws SlickException {
        Font font = new Font("Monospaced",Font.BOLD,32);
        ttf = new TrueTypeFont(font,false);
    }

    @Override
    public void update(GameContainer gc, StateBasedGame state, int ticks) throws SlickException {
        if(gc.getInput().isKeyDown(Input.KEY_ESCAPE)) {
            gc.exit();
        } else if(gc.getInput().isKeyDown(Input.KEY_ENTER) || gc.getInput().isMousePressed(0)) {
            state.enterState(Mindmelt.playScreen,new FadeOutTransition(),new FadeInTransition());
        }
    }

    @Override
    public void render(GameContainer gc, StateBasedGame state, Graphics g) throws SlickException {
        start = new Image("res/start.bmp",false);
        start.draw();
        g.setColor(Color.red);
        ttf.drawString(202,302, "Press Enter To Start",Color.black);
        ttf.drawString(200,300, "Press Enter To Start",Color.yellow);
        ttf.drawString(202,342, "Press Esc To Exit",Color.black);
        ttf.drawString(200,340, "Press Esc To Exit",Color.yellow);
    }
}
