# README

This repository is an attempt to create a dictionary of equations, and a system to
allow papers to be written to refer to a named equation in the dictionary.

The reasons for this effort are:

* To allow papers to generate equations that can provide a link for blind users that 
will read the equation and give a written English description of it because Braille
only allows for English letters, not all the symbols of mathematics. There is Braille math
but it is unclear how many people know it.

* To define a dictionary that documents each symbol and component used, much
as coding libraries document the parameters of functions, and classes.

* To make equations in scientific papers more readable to a wider audience by 
adding explanatory text. Originally, Kit and Dov have defined a two-level approach,
with a technical discussion aimed at people who know basic differential and integral
calculus. So the technical explanation of the equations should be written at a level
where a 2nd year undergraduate engineering student could learn the material.
The non-technical explanation is aimed at an 8th grade reading level with essentially no math background. That explanation cannot really deal with the
mathematical details but allows for an overview, giving the reader a sense of what
the equation is about, and gives links for the reader to learn the knowledge needed 
to understand the equation.

* There should also be cross-referencing, so that the reader can learn about other equations in the same field, or related concepts, and citations of the best educational resources in the area.

We are offering this first version as a proof of concept, and defining a few papers
starting with a Shannon paper. We are creating a mockup hoping to get feedback,
corrections, and suggestions for improvement. Kit has recruited a diverse group of
Bell Labs people, IEEE Accessibility Committee members, and friends. Dov has recruited
some students. Once consensus is reached that the concept is well-defined and is
working on a limited subset of papers, we will start to scale up the set of volunteers.

Our concept is to invite dozens, then hundreds of volunteers, to find the top
papers in each field, edit them to build LaTeX source code referencing the equation
dictionary, and to generate a library of papers that are accessible and link to the
dictionary. Once the project is scaled beyond a dozen volunteers, it does not make
sense to have everyone with write permission to this repo. At that point, we 
think it makes sense to fork the repo, and top level contributors will have write
permission, with newcomers editing forked repos, and making pull requests which
will be reviewed by top level contributors. Again, we are hoping to get feedback
on the concept, we cannot afford to have a bottleneck in the review process.

The model for this effort is distributed proofreaders, a website that was created for project Gutenberg to allow multiple volunteers, https://www.pgdp.net/c/acting in parallel to check each others' work, to edit and publish books in the public domain electronically.
The website for that effort is: https://www.pgdp.net/c/

A fundamental question is how to represent equations. At the moment, we are using LaTeX as it is the most popular format, used in LaTeX (obviously) for most papers,
and online resources using MathJax. MathML claims to be a better representation,
that LaTeX only does presentation whereas MathML represents the semantics of the equation as well. We have not yet seen an instance where we need an additional
functionality, but we have yet to read through the full MathML docs, perhaps that
will show cases where additional semantics are needed.

We are looking for a mapping for terms in equations that define pronunciation of
components. For example:

\[
    \frac{\partial f}{\partial t}   
\]

could be mapped to: "partial f with respect to t". A screen reader might just
read the names of the symbols, ie "nabla f nabla t" and not sure what it would do
for the division, probably nothing as that is a line not a symbol.

The repo is organized as follows:

* eqdict.dat: This is the dictionary of equations. It is a simple text file with
  one equation per line. The format is:
  <equation name>: <equation>
  where the equation is a LaTeX string.

* articles directory contains articles in LaTeX that will hopefully use the dictionary. At the moment, we have no LaTeX code written to generate, but we did
test our ability to read in a pdf of a paper, have ChatGPT reverse engineer LaTeX.
The first pass generated a one-page excerpt of a Shannon paper that didn't compile.
The second pass, telling the AI in the cursor editor to "fix" given the LaTeX source and the compile-time errors yielded a working paper. The only error we can
see is that one equation uses lowercase x rather than capital X.
    * Inside articles is refs/ offering the original papers we found. There is a problem that publishers of current papers might well protest against
    rewriting their papers and putting them on the internet. Generally authors
    will want their papers to be read, but the publishers often charge exorbitant fees for access. We believe putting this source material in the repo for now constitutes fair use, and some of the PDFs in there were already on the web but we realize that this is a potential legal problem going forward.
    The paper was compiled with
    ```shell
    pdflatex shannon_exerpt.tex
    ```
    Note that this requires a large installation of LaTeX on your computer, and that resolving citations which are not included in this snippet would require a more complicated build:
    ```shell
    pdflatex shannon_exerpt.tex
    bibtex   shannon_exerpt
    pdflatex shannon_exerpt.tex
    pdflatex shannon_exerpt.tex
    ```
    When we get paper(s) with full citations ready, a Makefile will be added to build them all with the single command:
    ```shell
    make
    ```

* xp directory contains experimental code, you can drop files in there if you are playing with ideas and there is no expectation that code in there will be working.
Now that we are opening this repo for multiple people, it is a better idea to create a branch and work in your private branch until you have something working, then merge it in.

* Bolzmann.md
  This is an example of a proposed 2nd-level dictionary entry. The equation itself is stored in eqdict.dat. The markdown file documents the parameters and terms of the equation, and then the user can click for more information if desired. We currently display everything for every user. It would be nice if the user could configure their reading level and get the appropriate level description.

* Bolzmann-full.md
  This is an example of the 3rd-level dictionary file with the explanations, cross-referencing, and citations. At the moment, clicking on the equation is envisioned to get to a markdown file of the same name, then clicking on the request for more info would get to the 3rd-level file.
