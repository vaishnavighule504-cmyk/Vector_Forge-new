*****observe*****

Shapes is the rule sheet that says "anything called a shape must be able to draw itself , say if a click hit it , and move" you  can't draw a shape that is just "A shape"

CircleShape , RectShape .. are real shapes that follow the rule sheet . 

Document - a box that holds all the shapes. --- isme polymorphism use hua hai

Canvas is the window on the screen. 

shape is abstract because -- 

destructor virtual -- hierarchical destruction is seen here so if the destructor was not the virtual then deleting circleshape through a shape pointer would have i think missed some -- cross check it 

Document::draw , the line s->draw(painter)

unique_ptr --> it means exactly one owner , the memory is freed automatically -- so the document owns it right?

shapeAt returns a plain Shape* --> cause abhi tak document hi sare shape ko own karta hai

override why????
override makes the compiler catch signature typos

   **** PHASE 2******

Why reset m_selected to nullptr after deleting? The Document freed the shape, so the pointer now points to memory that no longer holds a shape (a dangling pointer). Using it again would crash or behave unpredictably.
unique_ptr m_preview vs raw m_selected? m_preview owns the shape being dragged. On release, std::move hands that ownership to the Document. m_selected only borrows a shape the Document owns, so Canvas never deletes it.
Why update() and not paintEvent() directly? update() asks Qt to repaint when it's ready, and Qt then calls paintEvent with a valid painter. Calling paintEvent yourself skips that setup, and it can repaint many times for one change.

Add a fourth: "The mouse flow is press (start), move (update the preview or move the shape), release (commit the shape to the Document)."

Understanding check

Say these out loud without looking:

When I drag with the Circle tool, where does the circle live before I release? (m_preview, not yet in the Document.)
What happens to it on release? (Moved into the Document.)
Why does Document::draw draw the dashed box? (Because isSelected() is true for that shape.)

    ****ADDED UNDO REDO OPTIONS*****
What is the Command pattern? An action is turned into an object with redo() and undo(), so it can be stored, undone and replayed.
Why does DeleteCommand hold a unique_ptr? So the deleted shape stays alive for undo. If it were destroyed, there'd be nothing to restore.
Why do AddCommand's undo and redo pass ownership back and forth? Whoever isn't currently holding the shape in the document must own it, so there's always exactly one owner.
Why push(cmd, false) for moves? The drag already moved the shape live. Running redo() again would move it twice.
Why does a new action clear the redo list? The old "future" no longer matches the current state.
Why clearSelection() before undo? An undo can remove the selected shape from the document, and we don't want a stale highlight or pointer.

 ****phase 5*****
 What is the Factory pattern, and why use it here? ShapeFactory creates the right subclass from a type string, so the loading code doesn't depend on the concrete shape classes.
Why does Document::fromJson build into a temporary vector first? If the file is half-broken, the current drawing isn't destroyed. It either loads fully or not at all.
Why m_undo.clear() after loading? Old commands hold raw pointers to shapes that no longer exist, and using them would crash.
Why does toJson live in each shape instead of one big function? Polymorphism again. A new shape type adds its own toJson and one else if in the factory, and nothing else changes.
Why store the "version" number? So future file formats can still be read.


****phase 6****
Why does shapeAt loop from the end of the list in the linear version, and what does order do in the Quadtree version? Both give the topmost shape.
Why do straddling shapes stay in the parent node? They don't fit fully in one child, and putting them in two would create duplicates.
Why kMaxDepth? If many shapes sit at the same spot, splitting would never stop.
Why is the index mutable and rebuilt lazily? A cache is not logical state of the document, and rebuilding only when needed avoids doing it on every change.
What is the weakness? Rebuilding costs time after every change. An incremental update would be better, and you can mention that as an improvement.
Is it really O(log n)? On average, for evenly spread shapes. If everything piles into one spot, it degrades toward O(n). Say this in interviews, because it shows depth.