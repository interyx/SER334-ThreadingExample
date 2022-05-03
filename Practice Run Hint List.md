 # The Threading Assignment
 Ok, buckle up because this thing is a doozy.  At least it was for me.
 The full details are in the assignment PDF, so refer back to that for specifics, but I want to give an overview and identify some of the issues and gotchas that can pop up with this program to help build a design for it.
 
 ## Requirements:
 - Create a threaded algorithm to blur an image based on the "nearest-neighbor" box blur technique.  The average color values of a 3x3 square will be used to calculate each pixel's color data, which has the effect of blurring the image
 - Create a threaded algorithm that randomly draws random black holes spread in a normal distribution across the x and y axis while tinting the rest of the image a buttery yellow color.
 
 
 ## Strategy
 This is image manipulation again, but we're not actually testing on whether your solution to reading images worked.  The compiled object files for reading and writing images has been provided.  If your solution didn't quite work then you will be able to use these, but as you can't see the implementation you have to carefully consider the inputs and outputs of these functions to be sure you're doing the right thing.  Understand how the data flows between them and your program, open some test files to make sure you can read and write properly before continuing.  You don't want to be debugging multiple issues at once.
 
 This program sounds like a lot.  How do you even start?  Well, like any problem the trick is to break it into smaller and smaller pieces and then build the solution back up.
 
 Now, my strong advice is **not** to try and develop a threaded version of these algorithms straight out of the gate.  Trying to fix logic bugs and threading bugs at the same time is a recipe for pure frustration.  Get it working straight through first and then attempt to thread it.
 
 My first step for any problem, especially one that deals with 2D array matrices like this, is to draw it out.  What values do you need for a pixel in the middle?  What about on the edges?  And especially what about the corners?  Get some pseudocode down and trace it against your visual model, and really try to be like the computer and execute the instructions as written and not how you mean to interpret them.  When you've got it down on paper, then you can attempt to implement it.  Have a plan in place before you start typing!  You can always refer back to it while you're coding and getting an unexpected result and try to find logical errors or differences between your plan and your implementation.
 
 The box blur algorithm is pretty straightforward, but generalizing a solution that looks at that many 3x3 grids is a little tricky.  Make sure you check all your edges and try not to get too demoralized at the sight of a segmentation fault.  It's probably going to happen unless you test your handwritten code exhaustively and get it all down perfect.
 
 The cheese filter is one that I had a lot of trouble with.  The naive solution will have issues that will come out during threading, so you might want to keep the threading issues in mind while you're designing it.  I don't want to give too much away, but you're going to have huge problems if you have a black spot that crosses thread borders depending on how you calculate your circles.  There are at least two approaches I can think of to thread this and they are not the obvious one, so really stretch your brain and try to be creative to come up with a solution.
 
 The other issue is generating the circles which will take a little bit of geometry, but if you think about what a circle actually is it should come pretty quickly.  I have seen people try to come up with super elegant mathematical solutions for drawing circles, and if you want to go this route for geek points I can't fault you, but you don't actually get any assignment points for a fancy solution like that and it'll just take up development time.  The simple solution works well and you can work on stuff that actually matters.  Similar to this, don't worry about perfectly distributed random numbers, it doesn't need to be that finicky and precise.
 
 Once your straightforward solutions work you'll have to develop threaded versions.  I think the best way to do it is to set up the threads to run in sequence instead of series; if you have four threads, set it up so that the first one `join()`s before the next one launches.  Debugging four running threads at the same time is nearly impossible because multiple concurrent threads of execution are running instructions at the same time, so by slowing it down you can see what each thread is doing and where it's throwing errors.
 
 ## IDE Issues
 Getting stuff to compile with `pthread` in CLion is, in my experience, annoying.  I had it working when I was writing this, but coming back to my completed program I can't get it to run without throwing segmentation faults.  Compiling and running through `gcc` in the command line works fine.  This is the way I'd test it personally, don't waste time messing with the IDE.
 
 ## Compiler Issues
 Just like importing the math library, `gcc` needs to be explicitly told at compile time to link the threading library with the `-lpthread` switch at the end of the command.
 
 A lot of research is required for stuff like this.  As always the keys to success are:
 
 1. Start early
 2. Ask questions
 3. Attend the SI sessions
 
 Once you get this working, don't forget about the other assignment due this week.  It's not as hard as this one, and it's not as hard as it looks, so don't stress about that one too much.  Don't wait until the last minute either because it still takes a little bit of time to understand and then implement.
 
 Good luck!
