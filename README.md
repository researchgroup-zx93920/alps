# ALPS - In development :mountain:
Accelerated Linear Programming Solver (ALPS) is supposed to be a GPU accelerated LP solver


### Dependencies

- CoinUtils, Installation Instructions provided [here](https://github.com/researchgroup-zx93920/CoinUtils#using-coinbrew)
- C++ Boost, A description about boost and installation instructions is provided [here](https://www.boost.org/doc/libs/1_62_0/more/getting_started/unix-variants.html)

### Limitations

- Alps should only accept continuous domains (no integer varaibles)
- Alps because of limited scope should only expect LP in following standard form 
  
  ![equation](https://latex.codecogs.com/svg.image?\min&space;~&space;c^Tx&space;:&space;Ax&space;=&space;b;&space;x&space;\geq&space;0)
  
- Alps must accept the LP problem in minimization sense

#### Error Codes

Mohit is keeping a track (this is un-stable right now)

| Code  | Interpretation |
| ------------- | ------------- |
| -1  | Problems in reading the configuration |
| -2 | Unexpected/Unknown problem |
| -3  | Problems in File Format  |



