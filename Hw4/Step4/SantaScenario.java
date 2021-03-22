package Step4;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Semaphore;

import Step4.Santa.SantaState;

public class SantaScenario {

	public Santa santa;
	public List<Elf> elves;
	public List<Reindeer> reindeers;
	public boolean isDecember;
	public boolean isDone;
	public ArrayList<Elf> atDoor;
	public List<Elf> inTroubleList;
	public Semaphore santa_door_check;
	public Semaphore wake_santa;
	public Semaphore santa_help;
	
	public static void main(String args[]) {
		SantaScenario scenario = new SantaScenario();
		scenario.isDecember = false;
		scenario.isDone = false;
		scenario.atDoor = new ArrayList<>();
		scenario.inTroubleList = new ArrayList<>();

		scenario.santa_door_check = new Semaphore(0, true);
		scenario.wake_santa = new Semaphore(1, true);
		scenario.santa_help = new Semaphore(0, true);

		// create the participants
		// Santa
		scenario.santa = new Santa(scenario);
		Thread th = new Thread(scenario.santa);
		th.start();

		// The elves: in this case: 10
		scenario.elves = new ArrayList<>();
		for(int i = 0; i != 10; i++) {
			Elf elf = new Elf(i+1, scenario);
			scenario.elves.add(elf);
			th = new Thread(elf);
			th.start();
		}

		// The reindeer: in this case: 9
		scenario.reindeers = new ArrayList<>();
		for(int i=0; i != 9; i++) {
			Reindeer reindeer = new Reindeer(i+1, scenario);
			scenario.reindeers.add(reindeer);
			th = new Thread(reindeer);
			th.start();
		}

		// now, start the passing of time
		for(int day = 1; day < 500; day++) {  
			// wait a day
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			// turn on December
			if (day > (365 - 31)) {
				scenario.isDecember = true;
			}

			if(day > 370){
				scenario.santa.setTerminate(true);
				for(Elf e : scenario.elves){
					scenario.santa_door_check.release();
					e.setTerminate(true);
				}
					
				for(Reindeer r : scenario.reindeers)
					r.setTerminate(true);
			}

			if(scenario.inTroubleList.size() >= 3 && scenario.atDoor.isEmpty()){
				scenario.santa_door_check.release();
			}
			
			if(!scenario.atDoor.isEmpty() && scenario.atDoor.size() >= 3){
				scenario.santa_help.release();
			}

			// print out the state:
			System.out.println("***********  Day " + day + " *************************");
			scenario.santa.report();
			for(Elf elf: scenario.elves) {
				elf.report();
			}
			for(Reindeer reindeer: scenario.reindeers) {
				reindeer.report();
			}
		}
	}
	
	
	
}
