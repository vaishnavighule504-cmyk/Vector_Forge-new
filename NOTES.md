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