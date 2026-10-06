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

