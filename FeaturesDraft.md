##  Objects

```
object description ThingDescription {  
    // 'Processing' only knows what is explicitly in StorageAccessors
    methodgroup BufferProcessing uses BufferInfoAccessors {
        ProcessBuffer : void {
            GetBufferPtr()[...$d] = RandomNumber()
        }
    }

    methodgroup BufferInfoAccessors uses BufferInfo {
        GetBufferPtr => buffer[0...length]
    }

    // Inacessible to anything not 'using' it
    fields BufferInfo {
        length : Int
        buffer : Int[100]
    }

    fields Irrelevent {
        useless : Int
        junk : Garbage
    }
}

// Declare a new class using the description
// It only knows about Process() 

// Any fields
object class Description : 
    // Include only what we need for BufferProcessing
    ThingDescription(uses BufferProcessing)
{
    // Does not know about Irrelevent.
    // Infact, the compiler will not even allocate space for it.
}

function Testing {
    let thing = Thing()
    thing.ProcessBuffer()
}
```


## Enums


### Defining a *Sequential Enumeration* (Also known as a C style enum)
```
enum StatusCode {
    Ok,
    Warning,
    Fail
};

// Sugar for

const Ok = 0
const Warning = 1
const Fail = 2

&Ok // (Illegal. Enumerated values may not be addressed)
```

### Defining a *Bit Enumeration*
```
bitenum Flags {
    A,
    B,
    C
}

// Sugar for
const A = 1 << 0
const B = 1 << 1
const C = 1 << 2

```


### Defining a *Dynamic Enumeration*
```
const x = 0

enum StatusCode {
    Ok = x
    Fail = ~x,
}

// Sugar for:
const Ok = x
const Fail = ~x
```


## Argument lists
```
const arglist XYZ = (x, 1.0, 2.0)

const array = [XYZ]
const vector_a = Vector3(XYZ)
const vector_b = Vector3(-1.0, -2.0, XYZ[0])

// Sugar for
const array = [-1.5, 1.0, 2.0]
const vector_a = Vector3(0.0, 1.0, 0.0)
const vector_b = Vector3(-1.0, -2.0, 1.0)
```

## Bounded Pointers
```
// 'numbers' has at least 0 elements and may not be indexed beyond 9
function Use(numbers : *Int [0...10]) {
    numbers[0] = 0 // Ok
    numbers[10] = ... // Compile error
    
    // Dollar expressions may be used since we know its size
    numbers[...$n] = $n
    
    // Syntactic sugar for
    for (var i = 0; i < 10; i = i + 1) {
        numbers[i] = i
    }
}

function Use(count : Size, numbers : *Int [1...count]) {
    // ..
    numbers[5] = 123
}

// Is syntactic sugar for

function Use(count : Size, numbers : *Int [1...count]) {
    if count < 1 {
        // Error message
    }
    // ..
    
    if 5 < count then numbers[5] = 123
}

// Examples of creating a bounded pointer

// 'unsafe' must be used if we are creating a bounded pointer from an unbounded pointer
function Use(data : *Int) unsafe {
    data[...$d] = ... // Not possible. We don't know its size.
    
    const slice = data[0...10]
    slice[...$d] = ... // Ok
}

function Use(data : *Int [0..10]) {
    const bad_ptr = data[0...15] // Compile error
     
    //
    const len = Unknown()
    const checked_ptr = data[0...len]
    
    // Equivilent to
    const len = Unknown()
    if len > 10 then // Error
    const checked_ptr = data[0...len]
    
    checked_ptr[...$d] = ... // Ok
}

```


# For Loops

```
// C style
for (var x = 0; x < 10; x = x + 1) {

}

// N - Dimensional ranged-for loop
for ranged (x : Int in 0 ... 10, y : Int in 0 ... 10) {
    if Something(x, y) then break x
}
```
